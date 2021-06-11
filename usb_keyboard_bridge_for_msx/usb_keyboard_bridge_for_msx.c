// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Copyright (c) 2021 HRA! (t.hara)
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
// --------------------------------------------------------------------
//
//  2021年05月05日: SONY HB-F500 Support を追加
//      きんのじさん(@v9938 [twitter])のカスタムにより、HB-F500 で利用可能になりました
//      そのサポートコードを追加
//
//  May/5th/2021: Added the Sony HB-F500 Support.
//
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "bsp/board.h"
#include "tusb.h"
#include "hardware/uart.h"

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_ROW_TYPE
//		0: Y3-Y0  に ROWアドレスが指定される
//		1: Y11-Y0 に ROWアドレスをデコードした結果が指定される (ONE HOT)
//		2: X8-X4 に ROWアドレスが指定される、Y0にDIR(HB-F500)
//
//		0: ROW address is specified for Y3-Y0.
//		1: The result of decoding the ROW address is specified for Y11-Y0 (ONE HOT)
//		2: ROW address is specified at X8-X4, DIR at Y0 (HB-F500)
//
#define MSX_KEYMATRIX_ROW_TYPE 1

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_INV
//		0: Y11-Y0 は正論理である
//		1: Y11-Y0 は負論理である
//
//		0: Y11-Y0 is positive logic.
//		1: Y11-Y0 is negative logic.
//
#define MSX_KEYMATRIX_INV 1

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_Y_PULL_UP
//	MSX_KEYMATRIX_X_PULL_UP
//		0: Y0-Y11, X0-X8 の入力ピンに 内蔵 PULL UP/DOWN は使わない
//		1: Y0-Y11, X0-X8 の入力ピンに 内蔵 PULL UP を使う
//		2: Y0-Y11, X0-X8 の入力ピンに 内蔵 PULL DOWN を使う
//
//		0: Do not use the built-in PULL UP/DOWN on the Y0-Y11, X0-X8 input pins.
//		1: Use built-in PULL UP for Y0-Y11, X0-X8 input pins
//		2: Use built-in PULL DOWN for Y0-Y11, X0-X8 input pins
//
#define MSX_KEYMATRIX_Y_PULL_UP 1
#define MSX_KEYMATRIX_X_PULL_UP 0

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_ROW_PIN
//		Y0 の GPIO番号。Y1-Y11 は、ここからインクリメントするかたちの連番
//
//		GPIO number of Y0. Y1-Y11 are sequential numbers that are incremented from here.
//
#define MSX_KEYMATRIX_ROW_PIN 2

// --------------------------------------------------------------------
//	MSX_X_HIZ_SEL
//		0: X出力は H or L である
//		1: X出力は HiZ or L である
//
//		0: X output is H or L
//		1: X output is HiZ or L
//
#define MSX_X_HIZ_SEL 1

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_RESULT_PIN
//		X0 の GPIO番号。X1-X7 は、ここからインクリメントする形の連番
//
//		GPIO number of X0. X1-X7 are sequential numbers that are incremented from here.
//
#define MSX_KEYMATRIX_RESULT_PIN 14

// --------------------------------------------------------------------
//  MSX_CAPS_LED_PIN
//	    CAPS LED信号入力の GPIO番号。
//
//		GPIO number of the CAPS LED signal input.
//
#define MSX_CAPS_LED_PIN 26

// --------------------------------------------------------------------
//  MSX_KANA_LED_PIN
//		かな LED信号入力の GPIO番号。
//
//		GPIO number of the KANA LED signal input.
//
#define MSX_KANA_LED_PIN 27

// --------------------------------------------------------------------
//  KEYMAP_SEL_PIN
//	    KEYMAP_1ST と KEYMAP_2ND 切替スイッチの GPIO番号。
//			open : keymap_1st
//			GND  : keymap_2nd
//
//		GPIO number of the KEYMAP_1ST and KEYMAP_2ND selector switches.
//
#define KEYMAP_SEL_PIN 28

// --------------------------------------------------------------------
//  PAUSE_KEY_SUPPORT
//	    0: no
//      1: yes
//
#define PAUSE_KEY_SUPPORT 0

// --------------------------------------------------------------------
//	UART関連 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//	DEBUG_UART_ON
//		0: UARTにログを出さない
//		1: UARTにログを出す
//	※UARTデバッグ時にのみ 1 にしてください。
//	  これを 1 にすると処理が遅くなり、MSXにタイする応答が間に合いません。
//
#define DEBUG_UART_ON 0

// --------------------------------------------------------------------
//	X出力テストモード
//		0: 通常モード
//		1: キー入力の代わりに、自動的に 0, 1, 2, 3, 4, 5, 6, 7 を繰り返し
//		   入力するモード 
//	
//	1 にして、0→1→2 ... と MSX に入力されれば、Y検出と X出力は正常に機能 
//	入力される文字が違う文字なら Y検出が正常に出来ていない可能性があります。
//	入力される文字が 0,2,3,6,7 のように抜ける場合、Xの対応する信号が接続
//	出来ていない可能性があります。
//	何も入力されない場合、Xがすべて接続されていないか、Y0が接続されていない
//	可能性があります。
//
#define AUTO_OUTPUT_MODE	0

// --------------------------------------------------------------------
#define MAX_REPORT  4

static uint8_t report_count[ CFG_TUH_HID ];
static tuh_hid_report_info_t report_info_arr[ CFG_TUH_HID ][ MAX_REPORT ];

static bool connected_keyboard[ CFG_TUSB_HOST_DEVICE_MAX ] = { false };
static hid_keyboard_report_t usb_keyboard_report[ CFG_TUH_HID ] = { 0 };

// --------------------------------------------------------------------
typedef struct {
	int			y_code;
	char		x_code;
} KEYMAP_T;

// --------------------------------------------------------------------
//	keymap
#include "keymap_1st.h"
#include "keymap_2nd.h"

// --------------------------------------------------------------------
static uint8_t msx_key_matrix[16] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};

#if MSX_KEYMATRIX_ROW_TYPE == 2
	static int y_table[16] = {
		0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 
		0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF, 
	};
#endif

static uint8_t KeyLEDFlags = 0;

static bool key_pressed = false;
static uint8_t led_dev_addr = 0;

// --------------------------------------------------------------------
static void gpio_init_sub_pull_up( uint gpio, bool dir_out ) {
	gpio_init( gpio);
	gpio_set_dir( gpio, dir_out );
	gpio_pull_up( gpio );
}

// --------------------------------------------------------------------
static void initialization( void ) {
	uint8_t i;
	int gpio;

	board_init();
	tusb_init();

	//	Set the GPIO signal direction.
	for( i = 0; i < 12; i++ ) {		// Y0-Y11, 12bits
		gpio = MSX_KEYMATRIX_ROW_PIN + i;
		gpio_init( gpio );
		gpio_set_dir( gpio, GPIO_IN );
		#if MSX_KEYMATRIX_Y_PULL_UP == 1
			gpio_set_pulls( gpio, true, false );		//	PULL UP
		#elif MSX_KEYMATRIX_Y_PULL_UP == 2
			gpio_set_pulls( gpio, false, true );		//	PULL DOWN
		#else
			gpio_set_pulls( gpio, false, false );
		#endif
	}
	for( i = 0; i < 9; i++ ) {		// X0-X7 and PAUSE, 9bits
		gpio = MSX_KEYMATRIX_RESULT_PIN + i;
		gpio_init( gpio );
		#if MSX_X_HIZ_SEL == 0
			gpio_set_dir( gpio, GPIO_OUT );
		#else
			gpio_set_dir( gpio, GPIO_IN );
		#endif
		#if MSX_KEYMATRIX_X_PULL_UP == 1
			gpio_set_pulls( gpio, true, false );		//	PULL UP
		#elif MSX_KEYMATRIX_X_PULL_UP == 2
			gpio_set_pulls( gpio, false, true );		//	PULL DOWN
		#else
			gpio_set_pulls( gpio, false, false );
		#endif
	}
	// LED Input (CAPS)
	gpio_init_sub_pull_up( MSX_CAPS_LED_PIN, GPIO_IN );
	// LED Input (KANA)
	gpio_init_sub_pull_up( MSX_KANA_LED_PIN, GPIO_IN );
	// Keymap Select Input
	gpio_init_sub_pull_up( KEYMAP_SEL_PIN, GPIO_IN );
}

// --------------------------------------------------------------------
#if MSX_KEYMATRIX_ROW_TYPE == 1
	#include "encode.h"
#endif

// --------------------------------------------------------------------
void response_core( void ) {
	uint32_t matrix;
	int y;
	static const uint32_t x_mask = 0x0FF << MSX_KEYMATRIX_RESULT_PIN;
	#if DEBUG_UART_ON
		int i, j;
		char s_buffer[32];
	#endif

	//	Output X
	#if MSX_X_HIZ_SEL == 1 && MSX_KEYMATRIX_ROW_TYPE != 2
		gpio_put_masked( x_mask, 0 );
	#endif

	for( ;; ) {
		#if MSX_KEYMATRIX_ROW_TYPE == 2
			//	Wait until the DIR signal goes to L level.
			while( ( gpio_get_all() & (1 << MSX_KEYMATRIX_ROW_PIN) ) !=0 );
			//	Switch X[4:7] to input
			gpio_set_dir_in_masked( 0x0F0 << MSX_KEYMATRIX_RESULT_PIN );
			//	Wait until the DIR signal goes to H level.
			while( ( gpio_get_all() & (1 << MSX_KEYMATRIX_ROW_PIN) ) ==0 );

			// Get the Y address to be input to X[4:7].
			// Y address is in reverse BIT order, so use y_table to reverse it.
			y = y_table[ ( gpio_get_all() >> (MSX_KEYMATRIX_RESULT_PIN + 4) ) & 0x00F ];

			// Prepare for X output Set X[4:7] to output again.
			#if MSX_X_HIZ_SEL == 0
				gpio_set_dir_out_masked( 0x0FF << MSX_KEYMATRIX_RESULT_PIN );
			#else
				gpio_put_masked( x_mask, 0 );
			#endif
		#else
			//	Get Y
			#if MSX_KEYMATRIX_INV == 0
				y = (gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) & 0x0FFF;
			#else
				y = ((gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) & 0x0FFF) ^ 0x0FFF;
			#endif

			#if MSX_KEYMATRIX_ROW_TYPE == 1
				y = encode_table[ y ];
			#endif
		#endif

		//	Get the key matrix specified by Y address.
		matrix = (uint32_t)msx_key_matrix[ y & 0x0F ] << MSX_KEYMATRIX_RESULT_PIN;
		//	Output X
		#if MSX_X_HIZ_SEL == 0
			gpio_put_masked( x_mask, matrix );
		#else
			gpio_set_dir_masked( x_mask, ~matrix );
		#endif

		#if DEBUG_UART_ON
			printf( "\x1B" "7Y :76543210\r\n" );
			for( i = 0; i < 16; i++ ) {
				sprintf( s_buffer, "%2d:", i );
				matrix = msx_key_matrix[i];
				for( j = 0; j < 8; j++ ) {
					s_buffer[ j + 3 ] = (matrix & 0x80) ? '1' : '0';
					matrix <<= 1;
				}
				s_buffer[ 8 + 3 ] = '\r';
				s_buffer[ 9 + 3 ] = '\n';
				s_buffer[10 + 3 ] = '\0';
				printf( s_buffer );
			}
			printf( "\x1B" "8" );
			sleep_ms( 1000 );
		#endif
	}
}

// --------------------------------------------------------------------
//	Reflect hid_keyboard_report_t in key_matrix
#if AUTO_OUTPUT_MODE == 0
	void update_key_matrix( uint8_t *p_matrix, const hid_keyboard_report_t *p ) {
		uint8_t i;
		const KEYMAP_T *p_keymap, *p_keymap_item;
	
		// select keymap
		if( gpio_get( KEYMAP_SEL_PIN ) == 0 ) {
			p_keymap = keymap_2nd;
		}
		else {
			p_keymap = keymap_1st;
		}
	
		// keycode
		for( i = 0; i < 6; i++ ) {
			p_keymap_item = &p_keymap[ p->keycode[i] ];
			if( p_keymap_item->y_code == -1 ) {
				continue;
			}
			p_matrix[ p_keymap_item->y_code ] &= ~(1 << p_keymap_item->x_code);
			key_pressed = true;
		}
		// modifier
		for( i = 0; i < 8; i++ ) {
			if( (p->modifier & (1 << i)) == 0 ) {
				continue;
			}
			p_keymap_item = &p_keymap[ 0x100 + i ];
			if( p_keymap_item->y_code == -1 ) {
				continue;
			}
			p_matrix[ p_keymap_item->y_code ] &= ~(1 << p_keymap_item->x_code);
			key_pressed = true;
		}
	}
#else
	void update_key_matrix( uint8_t *p_matrix, const hid_keyboard_report_t *p ) {
		static int current_key_bit = 0;
		static uint32_t start_ms = 0;
		static uint32_t interval_ms = 1000;

		p_matrix[0] = (uint8_t) ~(1 << current_key_bit);

		if( board_millis() - start_ms < interval_ms ) {
			return; // not enough time
		}
		start_ms += interval_ms;
		current_key_bit = (current_key_bit + 1) & 7;
	}
#endif

// --------------------------------------------------------------------
void hid_task( void ) {
	uint8_t current_key_matrix[ sizeof(msx_key_matrix) ];

	#if AUTO_OUTPUT_MODE == 0
		int i;
		memset( current_key_matrix, 0xFF, sizeof(current_key_matrix) );
		key_pressed = false;
		for( i = 0; i < CFG_TUH_HID; i++ ) {
			update_key_matrix( current_key_matrix, &(usb_keyboard_report[i]) );
		}
		memcpy( (void*) msx_key_matrix, current_key_matrix, sizeof(current_key_matrix) );
	#else
		memset( current_key_matrix, 0xFF, sizeof(current_key_matrix) );
		update_key_matrix( current_key_matrix, &usb_keyboard_report );
		memcpy( (void*) msx_key_matrix, current_key_matrix, sizeof(current_key_matrix) );
	#endif
}

//--------------------------------------------------------------------+
void led_blinking_task( void ) {

	board_led_write( key_pressed );
}

// --------------------------------------------------------------------
void setKeyboardLeds( uint8_t Keyleds ) {

	if ( Keyleds != KeyLEDFlags ){
		KeyLEDFlags = Keyleds;

		tusb_control_request_t ledreq = {
			.bmRequestType_bit.recipient	= TUSB_REQ_RCPT_INTERFACE,
			.bmRequestType_bit.type			= TUSB_REQ_TYPE_CLASS,
			.bmRequestType_bit.direction	= TUSB_DIR_OUT,
			.bRequest						= HID_REQ_CONTROL_SET_REPORT,
			.wValue							= HID_REPORT_TYPE_OUTPUT << 8,
			.wIndex							= 0,    // Interface number
			.wLength						= sizeof (KeyLEDFlags),
		};

		if( led_dev_addr ) {
			tuh_control_xfer( led_dev_addr, &ledreq, &KeyLEDFlags, NULL );
		}
	}
}

// --------------------------------------------------------------------
//	KEYBOARD_LED_NUMLOCK    = TU_BIT(0), ///< Num Lock LED
//	KEYBOARD_LED_CAPSLOCK   = TU_BIT(1), ///< Caps Lock LED
//	KEYBOARD_LED_SCROLLLOCK = TU_BIT(2), ///< Scroll Lock LED
//	KEYBOARD_LED_COMPOSE    = TU_BIT(3), ///< Composition Mode
//	KEYBOARD_LED_KANA       = TU_BIT(4)  ///< Kana mode
//
void led_and_pause_key_task( void ) {
	static uint8_t Keyleds = 0;

	//	Pause Key
	#if PAUSE_KEY_SUPPORT
		gpio_put( MSX_KEYMATRIX_RESULT_PIN + 8, msx_key_matrix[ 12 ] & 1 );
	#endif

	//	KEYBOARD LED Process
	if( gpio_get( MSX_CAPS_LED_PIN ) == 0 ) {
		Keyleds |= KEYBOARD_LED_CAPSLOCK;
	}
	else {
		Keyleds &= KEYBOARD_LED_CAPSLOCK ^ 0xFF;
	}
	
	//	規格上はKANA LEDは存在するが、SCROLLLOCKで代用する
	//	(かなLED を搭載しているキーボードがあまり存在しないため)
	//
	//	KANA LED exists in the standard, but SCROLLLOCK is used instead.
	//	(Since there are not many keyboards with kana LEDs, it is not possible to use them.)
	//
	if( gpio_get( MSX_KANA_LED_PIN ) == 0 ) {
		Keyleds |= KEYBOARD_LED_SCROLLLOCK;
	}
	else {
		Keyleds &= KEYBOARD_LED_SCROLLLOCK ^ 0xFF;
	}
	setKeyboardLeds( Keyleds );
}

// --------------------------------------------------------------------
int main( void ) {

	initialization();
	multicore_launch_core1( response_core );

	for( ;; ) {
		tuh_task();
		hid_task();
		led_blinking_task();
		led_and_pause_key_task();
	}
	return 0;
}

// --------------------------------------------------------------------
//	Callback to be called when a keyboard is connected.
void tuh_hid_mount_cb( uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len ) {

	#if DEBUG_UART_ON
		printf( "tuh_hid_mount_cb( %d, %d, %p, %d );\n", dev_addr, instance, desc_report, desc_len );
	#endif

	if( led_dev_addr == 0 ) {
		led_dev_addr = dev_addr;
		KeyLEDFlags = 0;
	}
	connected_keyboard[ dev_addr - 1 ] = true;
	report_count[instance] = tuh_hid_parse_report_descriptor( report_info_arr[instance], MAX_REPORT, desc_report, desc_len );
}

// --------------------------------------------------------------------
//	Callback to be called when the keyboard is disconnected.
void tuh_hid_umount_cb( uint8_t dev_addr, uint8_t instance ) {
	int i;

	#if DEBUG_UART_ON
		printf( "tuh_hid_umount_cb( %d, %d );\n", dev_addr, instance );
	#endif

	connected_keyboard[ dev_addr - 1 ] = false;
	memset( &usb_keyboard_report[ instance ], 0, sizeof(hid_keyboard_report_t) );

	if( led_dev_addr == dev_addr ) {
		led_dev_addr = 0;
		for( i = 0; i < CFG_TUSB_HOST_DEVICE_MAX; i++ ) {
			if( connected_keyboard[ i ] ) {
				led_dev_addr = i + 1;
			}
		}
		KeyLEDFlags = 0;
	}
}

// --------------------------------------------------------------------
void tuh_hid_report_received_cb( uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len ) {

	(void) dev_addr;

	uint8_t const rpt_count = report_count[ instance ];
	tuh_hid_report_info_t* rpt_info_arr = report_info_arr[ instance ];
	tuh_hid_report_info_t* rpt_info = NULL;

	#if DEBUG_UART_ON
		printf( "tuh_hid_report_received_cb( %d, %d, %p, %d )\n", dev_addr, instance, report, len );
	#endif

	if ( rpt_count == 1 && rpt_info_arr[0].report_id == 0) {
		// Simple report without report ID as 1st byte
		rpt_info = &rpt_info_arr[0];
	} else {
		// Composite report, 1st byte is report ID, data starts from 2nd byte
		uint8_t const rpt_id = report[0];

		// Find report id in the arrray
		for( uint8_t i = 0; i < rpt_count; i++ ) {
			if( rpt_id == rpt_info_arr[i].report_id ) {
				rpt_info = &rpt_info_arr[i];
				break;
			}
		}

		report++;
		len--;
	}

	if( !rpt_info ) {
		#if DEBUG_UART_ON
			printf( "-- rpt_info == NULL\n" );
		#endif
		return;
	}

	#if DEBUG_UART_ON
		printf( "-- rpt_info->usage_page == %d\n", rpt_info->usage_page );
		printf( "-- rpt_info->usage == %d\n", rpt_info->usage );
	#endif
	if( rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP ) {
		if( rpt_info->usage == HID_USAGE_DESKTOP_KEYBOARD ) {
			memcpy( &usb_keyboard_report[ instance ], report, sizeof(hid_keyboard_report_t) );
		}
	}
}
