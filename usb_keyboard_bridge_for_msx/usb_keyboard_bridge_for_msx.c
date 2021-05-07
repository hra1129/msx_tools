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
//	MSX_KEYMATRIX_ROW_PULL_UP
//		0: Y0-Y11 の入力ピンに 内蔵 PULL UP/DOWN は使わない
//		1: Y0-Y11 の入力ピンに 内蔵 PULL UP を使う
//		2: Y0-Y11 の入力ピンに 内蔵 PULL DOWN を使う
//
//		0: Do not use the built-in PULL UP/DOWN on the Y0-Y11 input pins.
//		1: Use built-in PULL UP for Y0-Y11 input pins
//		2: Use built-in PULL DOWN for Y0-Y11 input pins
//
#define MSX_KEYMATRIX_ROW_PULL_UP 1

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_ROW_PIN
//		Y0 の GPIO番号。Y1-Y11 は、ここからインクリメントするかたちの連番
//
//		GPIO number of Y0. Y1-Y11 are sequential numbers that are incremented from here.
//
#define MSX_KEYMATRIX_ROW_PIN 2

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
#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define DEBUG_UART_ON 0

// --------------------------------------------------------------------
CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;

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

static int y_table[16] = {
	0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 
	0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF, 
};

static uint8_t KeyLEDFlags = 0;

// --------------------------------------------------------------------
static void gpio_init_sub( uint gpio, bool dir_out ) {
	gpio_init( gpio);
	gpio_set_dir( gpio, dir_out );
	#if MSX_KEYMATRIX_ROW_PULL_UP == 1
		gpio_pull_up( gpio );
	#elif MSX_KEYMATRIX_ROW_PULL_UP == 2
		gpio_pull_down( gpio );
	#endif
}

// --------------------------------------------------------------------
static void gpio_init_sub_pull_up( uint gpio, bool dir_out ) {
	gpio_init( gpio);
	gpio_set_dir( gpio, dir_out );
	gpio_pull_up( gpio );
}

// --------------------------------------------------------------------
static void initialization( void ) {
	uint8_t i;

	board_init();

	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
	uart_set_hw_flow(UART_ID, false, false);
	uart_set_format(UART_ID, 8, 2, UART_PARITY_NONE);

	tusb_init();

	//	Set the GPIO signal direction.
	for( i = 0; i < 12; i++ ) {		// Y0-Y11, 12bits
		gpio_init_sub( MSX_KEYMATRIX_ROW_PIN + i, GPIO_IN );
	}
	for( i = 0; i < 9; i++ ) {		// X0-X7 and PAUSE, 9bits
		gpio_init_sub( MSX_KEYMATRIX_RESULT_PIN + i, GPIO_OUT );
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

	static int inline encode( int y ) {
		return encode_table[y];
	}
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
			gpio_set_dir_out_masked( 0x0FF << MSX_KEYMATRIX_RESULT_PIN );

		#else
			//	Get Y
			#if MSX_KEYMATRIX_INV == 0
				y = (gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) & 0x0FFF;
			#else
				y = ((gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) & 0x0FFF) ^ 0x0FFF;
			#endif

			#if MSX_KEYMATRIX_ROW_TYPE == 1
				y = encode( y );
			#endif
		#endif

		//	Get the key matrix specified by Y address.
		matrix = (uint32_t)msx_key_matrix[ y & 0x0F ] << MSX_KEYMATRIX_RESULT_PIN;
		//	Output X
		gpio_put_masked( x_mask, matrix );

		#if DEBUG_UART_ON
			uart_puts( UART_ID, "\x1B" "7Y :76543210\r\n" );
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
				uart_puts( UART_ID, s_buffer );
			}
			uart_puts( UART_ID, "\x1B" "8" );
			sleep_ms( 1000 );
		#endif
	}
}

// --------------------------------------------------------------------
//	Reflect hid_keyboard_report_t in key_matrix
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
	}
}

// --------------------------------------------------------------------
void hid_task( void ) {
	uint8_t const dev_addr = 1;
	uint8_t current_key_matrix[ sizeof(msx_key_matrix) ];

	if( !tuh_hid_keyboard_is_mounted( dev_addr ) ) return;

	if( tuh_hid_keyboard_get_report( dev_addr, &usb_keyboard_report ) == TUSB_ERROR_NONE ) {
		memset( current_key_matrix, 0xFF, sizeof(current_key_matrix) );
		update_key_matrix( current_key_matrix, &usb_keyboard_report );
		memcpy( (void*) msx_key_matrix, current_key_matrix, sizeof(current_key_matrix) );
	}
}

//--------------------------------------------------------------------+
void led_blinking_task( void ) {
    const uint32_t interval_ms = 250;
    static uint32_t start_ms = 0;

    static bool led_state = false;

    // Blink every interval ms
    if (board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

// --------------------------------------------------------------------
void setKeyboardLeds( uint8_t Keyleds ) {
	if ( Keyleds != KeyLEDFlags ){
		uint8_t const addr = 1;
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

		tuh_control_xfer( addr, &ledreq, &KeyLEDFlags, NULL );
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
		led_blinking_task();
		hid_task();
		led_and_pause_key_task();
	}
	return 0;
}

// --------------------------------------------------------------------
//	Callback to be called when a keyboard is connected.
void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr) {
	(void) dev_addr;
}

// --------------------------------------------------------------------
//	Callback to be called when the keyboard is disconnected.
void tuh_hid_keyboard_unmounted_cb( uint8_t dev_addr ) {
	(void) dev_addr;
}

// --------------------------------------------------------------------
//	An interrupt that is called when a key on the keyboard is pressed or released.
void tuh_hid_keyboard_isr( uint8_t dev_addr, xfer_result_t event ) {
	(void) dev_addr;
	(void) event;
}
