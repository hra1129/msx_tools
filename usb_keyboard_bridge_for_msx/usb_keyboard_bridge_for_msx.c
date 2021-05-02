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
//
#define MSX_KEYMATRIX_ROW_TYPE 1

//	MSX_KEYMATRIX_INV
//		0: Y11-Y0 は正論理である
//		1: Y11-Y0 は負論理である
//
#define MSX_KEYMATRIX_INV 1

//	MSX_KEYMATRIX_ROW_PULL_UP
//		0: Y0-Y11 の入力ピンに 内蔵 PULL UP/DOWN は使わない
//		1: Y0-Y11 の入力ピンに 内蔵 PULL UP を使う
//		2: Y0-Y11 の入力ピンに 内蔵 PULL DOWN を使う
//
#define MSX_KEYMATRIX_ROW_PULL_UP 1

//	MSX_KEYMATRIX_ROW_PIN
//		Y0 の GPIO番号。Y1-Y11 は、ここからインクリメントするかたちの連番
//
#define MSX_KEYMATRIX_ROW_PIN 2

//	MSX_KEYMATRIX_RESULT_PIN
//		X0 の GPIO番号。X1-X7 は、ここからインクリメントする形の連番
//
#define MSX_KEYMATRIX_RESULT_PIN 14

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
//		keymap[ USBキーコード ] = { y_code, x_code }
//			y_code:
//				MSXキーマトリクスの行番号 0～15。
//				-1 にすると、そのキーは無視される。
//			x_code: 
//				MSXキーマトリクスのビット番号 0～7。
//
static const KEYMAP_T keymap[] = {
	{ -1, 0 },		//	HID_KEY_NONE               0x00
	{ -1, 0 },		//	                           0x01
	{ -1, 0 },		//	                           0x02
	{ -1, 0 },		//	                           0x03
	{  2, 6 },		//	HID_KEY_A                  0x04
	{  2, 7 },		//	HID_KEY_B                  0x05
	{  3, 0 },		//	HID_KEY_C                  0x06
	{  3, 1 },		//	HID_KEY_D                  0x07
	{  3, 2 },		//	HID_KEY_E                  0x08
	{  3, 3 },		//	HID_KEY_F                  0x09
	{  3, 4 },		//	HID_KEY_G                  0x0A
	{  3, 5 },		//	HID_KEY_H                  0x0B
	{  3, 6 },		//	HID_KEY_I                  0x0C
	{  3, 7 },		//	HID_KEY_J                  0x0D
	{  4, 0 },		//	HID_KEY_K                  0x0E
	{  4, 1 },		//	HID_KEY_L                  0x0F
	{  4, 2 },		//	HID_KEY_M                  0x10
	{  4, 3 },		//	HID_KEY_N                  0x11
	{  4, 4 },		//	HID_KEY_O                  0x12
	{  4, 5 },		//	HID_KEY_P                  0x13
	{  4, 6 },		//	HID_KEY_Q                  0x14
	{  4, 7 },		//	HID_KEY_R                  0x15
	{  5, 0 },		//	HID_KEY_S                  0x16
	{  5, 1 },		//	HID_KEY_T                  0x17
	{  5, 2 },		//	HID_KEY_U                  0x18
	{  5, 3 },		//	HID_KEY_V                  0x19
	{  5, 4 },		//	HID_KEY_W                  0x1A
	{  5, 5 },		//	HID_KEY_X                  0x1B
	{  5, 6 },		//	HID_KEY_Y                  0x1C
	{  5, 7 },		//	HID_KEY_Z                  0x1D
	{  0, 1 },		//	HID_KEY_1                  0x1E
	{  0, 2 },		//	HID_KEY_2                  0x1F
	{  0, 3 },		//	HID_KEY_3                  0x20
	{  0, 4 },		//	HID_KEY_4                  0x21
	{  0, 5 },		//	HID_KEY_5                  0x22
	{  0, 6 },		//	HID_KEY_6                  0x23
	{  0, 7 },		//	HID_KEY_7                  0x24
	{  1, 0 },		//	HID_KEY_8                  0x25
	{  1, 1 },		//	HID_KEY_9                  0x26
	{  0, 0 },		//	HID_KEY_0                  0x27
	{  7, 7 },		//	HID_KEY_RETURN             0x28
	{  7, 2 },		//	HID_KEY_ESCAPE             0x29
	{  7, 5 },		//	HID_KEY_BACKSPACE          0x2A
	{  7, 3 },		//	HID_KEY_TAB                0x2B
	{  8, 0 },		//	HID_KEY_SPACE              0x2C
	{  1, 2 },		//	HID_KEY_MINUS              0x2D
	{  1, 2 },		//	HID_KEY_EQUAL              0x2E
	{  1, 0 },		//	HID_KEY_BRACKET_LEFT       0x2F
	{  1, 1 },		//	HID_KEY_BRACKET_RIGHT      0x30
	{  1, 4 },		//	HID_KEY_BACKSLASH          0x31
	{ -1, 0 },		//	HID_KEY_EUROPE_1           0x32
	{  1, 7 },		//	HID_KEY_SEMICOLON          0x33
	{  0, 7 },		//	HID_KEY_APOSTROPHE         0x34
	{  7, 2 },		//	全/半                      0x35
	{  2, 2 },		//	HID_KEY_COMMA              0x36
	{  2, 3 },		//	HID_KEY_PERIOD             0x37
	{  2, 4 },		//	HID_KEY_SLASH              0x38
	{  6, 3 },		//	HID_KEY_CAPS_LOCK          0x39
	{  6, 5 },		//	HID_KEY_F1                 0x3A
	{  6, 6 },		//	HID_KEY_F2                 0x3B
	{  6, 7 },		//	HID_KEY_F3                 0x3C
	{  7, 0 },		//	HID_KEY_F4                 0x3D
	{  7, 1 },		//	HID_KEY_F5                 0x3E
	{  7, 4 },		//	HID_KEY_F6                 0x3F  STOP
	{  8, 1 },		//	HID_KEY_F7                 0x40  CLS/HOME
	{  7, 6 },		//	HID_KEY_F8                 0x41  SELECT
	{  8, 2 },		//	HID_KEY_F9                 0x42  INS
	{  8, 3 },		//	HID_KEY_F10                0x43  DEL
	{ -1, 0 },		//	HID_KEY_F11                0x44
	{ -1, 0 },		//	HID_KEY_F12                0x45
	{ -1, 0 },		//	HID_KEY_PRINT_SCREEN       0x46
	{ -1, 0 },		//	HID_KEY_SCROLL_LOCK        0x47
	{ 12, 0 },		//	HID_KEY_PAUSE              0x48  PAUSE
	{  8, 2 },		//	HID_KEY_INSERT             0x49
	{  8, 1 },		//	HID_KEY_HOME               0x4A
	{ -1, 0 },		//	HID_KEY_PAGE_UP            0x4B
	{  8, 3 },		//	HID_KEY_DELETE             0x4C
	{  7, 4 },		//	HID_KEY_END                0x4D  STOP
	{ -1, 0 },		//	HID_KEY_PAGE_DOWN          0x4E
	{  8, 7 },		//	HID_KEY_ARROW_RIGHT        0x4F
	{  8, 4 },		//	HID_KEY_ARROW_LEFT         0x50
	{  8, 6 },		//	HID_KEY_ARROW_DOWN         0x51
	{  8, 5 },		//	HID_KEY_ARROW_UP           0x52
	{ -1, 0 },		//	HID_KEY_NUM_LOCK           0x53
	{  9, 2 },		//	HID_KEY_KEYPAD_DIVIDE      0x54
	{  9, 0 },		//	HID_KEY_KEYPAD_MULTIPLY    0x55
	{ 10, 5 },		//	HID_KEY_KEYPAD_SUBTRACT    0x56
	{  9, 1 },		//	HID_KEY_KEYPAD_ADD         0x57
	{ -1, 0 },		//	HID_KEY_KEYPAD_ENTER       0x58
	{  9, 4 },		//	HID_KEY_KEYPAD_1           0x59
	{  9, 5 },		//	HID_KEY_KEYPAD_2           0x5A
	{  9, 6 },		//	HID_KEY_KEYPAD_3           0x5B
	{  9, 7 },		//	HID_KEY_KEYPAD_4           0x5C
	{ 10, 0 },		//	HID_KEY_KEYPAD_5           0x5D
	{ 10, 1 },		//	HID_KEY_KEYPAD_6           0x5E
	{ 10, 2 },		//	HID_KEY_KEYPAD_7           0x5F
	{ 10, 3 },		//	HID_KEY_KEYPAD_8           0x60
	{ 10, 4 },		//	HID_KEY_KEYPAD_9           0x61
	{  9, 4 },		//	HID_KEY_KEYPAD_0           0x62
	{ -1, 0 },		//	HID_KEY_KEYPAD_DECIMAL     0x63
	{ -1, 0 },		//	HID_KEY_EUROPE_2           0x64
	{  6, 2 },		//	HID_KEY_APPLICATION        0x65  GRAPH
	{ -1, 0 },		//	HID_KEY_POWER              0x66
	{ -1, 0 },		//	HID_KEY_KEYPAD_EQUAL       0x67
	{ -1, 0 },		//	HID_KEY_F13                0x68
	{ -1, 0 },		//	HID_KEY_F14                0x69
	{ -1, 0 },		//	HID_KEY_F15                0x6A
	{ -1, 0 },		//	                           0x6B
	{ -1, 0 },		//	                           0x6C
	{ -1, 0 },		//	                           0x6D
	{ -1, 0 },		//	                           0x6E
	{ -1, 0 },		//	                           0x6F
	{ -1, 0 },		//	                           0x70
	{ -1, 0 },		//	                           0x71
	{ -1, 0 },		//	                           0x72
	{ -1, 0 },		//	                           0x73
	{ -1, 0 },		//	                           0x74
	{ -1, 0 },		//	                           0x75
	{ -1, 0 },		//	                           0x76
	{ -1, 0 },		//	                           0x77
	{ -1, 0 },		//	                           0x78
	{ -1, 0 },		//	                           0x79
	{ -1, 0 },		//	                           0x7A
	{ -1, 0 },		//	                           0x7B
	{ -1, 0 },		//	                           0x7C
	{ -1, 0 },		//	                           0x7D
	{ -1, 0 },		//	                           0x7E
	{ -1, 0 },		//	                           0x7F
	{ -1, 0 },		//	                           0x80
	{ -1, 0 },		//	                           0x81
	{ -1, 0 },		//	                           0x82
	{ -1, 0 },		//	                           0x83
	{ -1, 0 },		//	                           0x84
	{ -1, 0 },		//	                           0x85
	{ -1, 0 },		//	                           0x86
	{ -1, 0 },		//	                           0x87
	{  6, 4 },		//	かな                       0x88
	{ 11, 1 },		//	変換                       0x8A  実行
	{ 11, 3 },		//	無変換                     0x8B  取消
	{ -1, 0 },		//	                           0x8C
	{ -1, 0 },		//	                           0x8D
	{ -1, 0 },		//	                           0x8E
	{ -1, 0 },		//	                           0x8F
	{ -1, 0 },		//	                           0x90
	{ -1, 0 },		//	                           0x91
	{ -1, 0 },		//	                           0x92
	{ -1, 0 },		//	                           0x93
	{ -1, 0 },		//	                           0x94
	{ -1, 0 },		//	                           0x95
	{ -1, 0 },		//	                           0x96
	{ -1, 0 },		//	                           0x97
	{ -1, 0 },		//	                           0x98
	{ -1, 0 },		//	                           0x99
	{ -1, 0 },		//	                           0x9A
	{ -1, 0 },		//	                           0x9B
	{ -1, 0 },		//	                           0x9C
	{ -1, 0 },		//	                           0x9D
	{ -1, 0 },		//	                           0x9E
	{ -1, 0 },		//	                           0x9F
	{ -1, 0 },		//	                           0xA0
	{ -1, 0 },		//	                           0xA1
	{ -1, 0 },		//	                           0xA2
	{ -1, 0 },		//	                           0xA3
	{ -1, 0 },		//	                           0xA4
	{ -1, 0 },		//	                           0xA5
	{ -1, 0 },		//	                           0xA6
	{ -1, 0 },		//	                           0xA7
	{ -1, 0 },		//	                           0xA8
	{ -1, 0 },		//	                           0xA9
	{ -1, 0 },		//	                           0xAA
	{ -1, 0 },		//	                           0xAB
	{ -1, 0 },		//	                           0xAC
	{ -1, 0 },		//	                           0xAD
	{ -1, 0 },		//	                           0xAE
	{ -1, 0 },		//	                           0xAF
	{ -1, 0 },		//	                           0xB0
	{ -1, 0 },		//	                           0xB1
	{ -1, 0 },		//	                           0xB2
	{ -1, 0 },		//	                           0xB3
	{ -1, 0 },		//	                           0xB4
	{ -1, 0 },		//	                           0xB5
	{ -1, 0 },		//	                           0xB6
	{ -1, 0 },		//	                           0xB7
	{ -1, 0 },		//	                           0xB8
	{ -1, 0 },		//	                           0xB9
	{ -1, 0 },		//	                           0xBA
	{ -1, 0 },		//	                           0xBB
	{ -1, 0 },		//	                           0xBC
	{ -1, 0 },		//	                           0xBD
	{ -1, 0 },		//	                           0xBE
	{ -1, 0 },		//	                           0xBF
	{ -1, 0 },		//	                           0xC0
	{ -1, 0 },		//	                           0xC1
	{ -1, 0 },		//	                           0xC2
	{ -1, 0 },		//	                           0xC3
	{ -1, 0 },		//	                           0xC4
	{ -1, 0 },		//	                           0xC5
	{ -1, 0 },		//	                           0xC6
	{ -1, 0 },		//	                           0xC7
	{ -1, 0 },		//	                           0xC8
	{ -1, 0 },		//	                           0xC9
	{ -1, 0 },		//	                           0xCA
	{ -1, 0 },		//	                           0xCB
	{ -1, 0 },		//	                           0xCC
	{ -1, 0 },		//	                           0xCD
	{ -1, 0 },		//	                           0xCE
	{ -1, 0 },		//	                           0xCF
	{ -1, 0 },		//	                           0xD0
	{ -1, 0 },		//	                           0xD1
	{ -1, 0 },		//	                           0xD2
	{ -1, 0 },		//	                           0xD3
	{ -1, 0 },		//	                           0xD4
	{ -1, 0 },		//	                           0xD5
	{ -1, 0 },		//	                           0xD6
	{ -1, 0 },		//	                           0xD7
	{ -1, 0 },		//	                           0xD8
	{ -1, 0 },		//	                           0xD9
	{ -1, 0 },		//	                           0xDA
	{ -1, 0 },		//	                           0xDB
	{ -1, 0 },		//	                           0xDC
	{ -1, 0 },		//	                           0xDD
	{ -1, 0 },		//	                           0xDE
	{ -1, 0 },		//	                           0xDF
	{  6, 1 },		//	HID_KEY_CONTROL_LEFT       0xE0
	{  6, 0 },		//	HID_KEY_SHIFT_LEFT         0xE1
	{  6, 2 },		//	HID_KEY_ALT_LEFT           0xE2
	{ -1, 0 },		//	HID_KEY_GUI_LEFT           0xE3
	{  6, 1 },		//	HID_KEY_CONTROL_RIGHT      0xE4
	{  6, 0 },		//	HID_KEY_SHIFT_RIGHT        0xE5
	{  6, 2 },		//	HID_KEY_ALT_RIGHT          0xE6
	{ -1, 0 },		//	HID_KEY_GUI_RIGHT          0xE7
	{ -1, 0 },		//	                           0xE8
	{ -1, 0 },		//	                           0xE9
	{ -1, 0 },		//	                           0xEA
	{ -1, 0 },		//	                           0xEB
	{ -1, 0 },		//	                           0xEC
	{ -1, 0 },		//	                           0xED
	{ -1, 0 },		//	                           0xEE
	{ -1, 0 },		//	                           0xEF
	{ -1, 0 },		//	                           0xF0
	{ -1, 0 },		//	                           0xF1
	{ -1, 0 },		//	                           0xF2
	{ -1, 0 },		//	                           0xF3
	{ -1, 0 },		//	                           0xF4
	{ -1, 0 },		//	                           0xF5
	{ -1, 0 },		//	                           0xF6
	{ -1, 0 },		//	                           0xF7
	{ -1, 0 },		//	                           0xF8
	{ -1, 0 },		//	                           0xF9
	{ -1, 0 },		//	                           0xFA
	{ -1, 0 },		//	                           0xFB
	{ -1, 0 },		//	                           0xFC
	{ -1, 0 },		//	                           0xFD
	{ -1, 0 },		//	                           0xFE
	{ -1, 0 },		//	                           0xFF
	{  6, 1 },		//	HID_KEY_CONTROL_LEFT       modifier 0
	{  6, 0 },		//	HID_KEY_SHIFT_LEFT         modifier 1
	{  6, 2 },		//	HID_KEY_ALT_LEFT           modifier 2
	{ -1, 0 },		//	HID_KEY_GUI_LEFT           modifier 3
	{  6, 1 },		//	HID_KEY_CONTROL_RIGHT      modifier 4
	{  6, 0 },		//	HID_KEY_SHIFT_RIGHT        modifier 5
	{  6, 2 },		//	HID_KEY_ALT_RIGHT          modifier 6
	{ -1, 0 },		//	HID_KEY_GUI_RIGHT          modifier 7
};

// --------------------------------------------------------------------
static uint8_t msx_key_matrix[16] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};

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

	//	GPIOの信号の向きを設定
	for( i = 0; i < 12; i++ ) {		// Y0-Y11, 12bits
		gpio_set_dir( MSX_KEYMATRIX_ROW_PIN + i, GPIO_IN );
		#if MSX_KEYMATRIX_ROW_PULL_UP == 1
			gpio_pull_up( MSX_KEYMATRIX_ROW_PIN + i );
		#elif MSX_KEYMATRIX_ROW_PULL_UP == 2
			gpio_pull_down( MSX_KEYMATRIX_ROW_PIN + i );
		#endif
	}
	for( i = 0; i < 9; i++ ) {		// X0-X7 and PAUSE, 9bits
		gpio_set_dir( MSX_KEYMATRIX_RESULT_PIN + i, GPIO_OUT );
	}
}

// --------------------------------------------------------------------
#if MSX_KEYMATRIX_ROW_TYPE == 1
	static int inline encode( int y ) {
		int i;

		for( i = 0; i < 12; i++ ) {
			if( y == (1 << i) ) {
				return i;
			}
		}
		return 15;
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
		//	Yを得る
		#if MSX_KEYMATRIX_INV == 0
			y = (gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) & 0x0FFF;
		#else
			y = ((gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) & 0x0FFF) ^ 0x0FFF;
		#endif

		#if MSX_KEYMATRIX_ROW_TYPE == 1
			y = encode( y );
		#endif

		//	Xを出力する
		matrix = (int)msx_key_matrix[ y & 0x0F ] << MSX_KEYMATRIX_RESULT_PIN;
		gpio_put_masked( x_mask, matrix );

		gpio_put( MSX_KEYMATRIX_RESULT_PIN + 8, msx_key_matrix[ 12 ] & 1 );

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
//	hid_keyboard_report_t を key_matrix に反映させる
void update_key_matrix( uint8_t *p_matrix, const hid_keyboard_report_t *p ) {
	uint8_t i;
	const KEYMAP_T *p_keymap;

	// keycode
	for( i = 0; i < 6; i++ ) {
		p_keymap = &keymap[ p->keycode[i] ];
		if( p_keymap->y_code == -1 ) {
			continue;
		}
		p_matrix[ p_keymap->y_code ] &= ~(1 << p_keymap->x_code);
	}
	// modifier
	for( i = 0; i < 8; i++ ) {
		if( (p->modifier & (1 << i)) == 0 ) {
			continue;
		}
		p_keymap = &keymap[ 0x100 + i ];
		if( p_keymap->y_code == -1 ) {
			continue;
		}
		p_matrix[ p_keymap->y_code ] &= ~(1 << p_keymap->x_code);
	}
}

// --------------------------------------------------------------------
void hid_task(void) {
	uint8_t const dev_addr = 1;
	uint8_t current_key_matrix[ sizeof(msx_key_matrix) ];

	if( !tuh_hid_keyboard_is_mounted( dev_addr ) ) return;

	if( tuh_hid_keyboard_get_report( dev_addr, &usb_keyboard_report ) == TUSB_ERROR_NONE ) {
		memset( current_key_matrix, 0xFF, sizeof(current_key_matrix) );
		update_key_matrix( current_key_matrix, &usb_keyboard_report );
		memcpy( msx_key_matrix, current_key_matrix, sizeof(current_key_matrix) );
	}
}

//--------------------------------------------------------------------+
void led_blinking_task(void) {
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
int main(void) {

	initialization();
	multicore_launch_core1( response_core );

	for( ;; ) {
		tuh_task();
		led_blinking_task();
		hid_task();
	}
	return 0;
}

// --------------------------------------------------------------------
//	キーボードが接続されたときに呼び出されるコールバック
void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr) {
	(void) dev_addr;
}

// --------------------------------------------------------------------
//	キーボードが切断されたときに呼び出されるコールバック
void tuh_hid_keyboard_unmounted_cb( uint8_t dev_addr ) {
	(void) dev_addr;
}

// --------------------------------------------------------------------
//	キーボードのキーが押されたり、放されたりすると呼び出される割り込み
void tuh_hid_keyboard_isr( uint8_t dev_addr, xfer_result_t event ) {
	(void) dev_addr;
	(void) event;
}
