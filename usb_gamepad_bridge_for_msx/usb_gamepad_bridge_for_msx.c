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
#include "pico/time.h"
#include "bsp/board.h"
#include "tusb.h"
#include "hardware/uart.h"

// --------------------------------------------------------------------
//	MSX ÇÃ è„ÅAâ∫ÅAç∂ÅAâEÅAAÅAB ÇÃÉ{É^ÉìÇ…ëŒâûÇ∑ÇÈ GPIOÉsÉìÇÃäJénî‘çÜ
//
//	Start number of the GPIO pin corresponding to the Up, Down, Left, 
//	Right, A, and B buttons on the MSX.
//
#define MSX_BUTTON_PIN 2

// --------------------------------------------------------------------
//	MSX Ç©ÇÁóàÇÈ SELêMçÜÇÃ GPIOÉsÉìî‘çÜ
//
//	GPIO pin number of the SEL signal coming from the MSX
//
#define MSX_SEL_PIN 8

// --------------------------------------------------------------------
//	SELêMçÜÇÃò_óù
//		0: ïâò_óù (MSX Joymega)
//		1: ê≥ò_óù (MegaDrive)
//
//	Logic of SEL signal
//		0: negative logic (MSX Joymega)
//		1: positive logic (MegaDrive)
//
#define MSX_SEL_LOGIC 0

// --------------------------------------------------------------------
//	X,Yé≤ä¥ìx
//
//	X,Y axis sensitivity
//
#define	LEFT_THRESHOLD		64
#define	RIGHT_THRESHOLD		192
#define	TOP_THRESHOLD		64
#define	BOTTOM_THRESHOLD	192

// --------------------------------------------------------------------
//	BUTTON MAP
//
#define A_BUTTON			TU_BIT(2)
#define B_BUTTON			TU_BIT(0)
#define C_BUTTON			TU_BIT(1)
#define X_BUTTON			TU_BIT(5)
#define Y_BUTTON			TU_BIT(3)
#define Z_BUTTON			TU_BIT(4)
#define START_BUTTON		TU_BIT(7)
#define MODE_BUTTON			TU_BIT(6)

// --------------------------------------------------------------------
#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define DEBUG_UART_ON 0

// --------------------------------------------------------------------
#if MSX_SEL_LOGIC == 0
	#define MSX_SEL_H true
	#define MSX_SEL_L false
#else
	#define MSX_SEL_H false
	#define MSX_SEL_L true
#endif

// --------------------------------------------------------------------
static uint8_t volatile joymega_matrix[5] = {
	0x33, 0x3F, 0x03, 0x3F, 0x3F
};

// --------------------------------------------------------------------
CFG_TUSB_MEM_SECTION static hid_gamepad_report_t usb_gamepad_report;

// --------------------------------------------------------------------
static void initialization( void ) {
	uint8_t i;

	board_init();

	#if DEBUG_UART_ON
		uart_init(UART_ID, BAUD_RATE);
		gpio_set_function( UART_TX_PIN, GPIO_FUNC_UART );
		gpio_set_function( UART_RX_PIN, GPIO_FUNC_UART );
		uart_set_hw_flow( UART_ID, false, false );
		uart_set_format( UART_ID, 8, 2, UART_PARITY_NONE );
	#endif

	tusb_init();

	//	GPIOÇÃêMçÜÇÃå¸Ç´Çê›íË
	for( i = 0; i < 6; i++ ) {
		gpio_init( MSX_BUTTON_PIN + i );
		gpio_set_dir( MSX_BUTTON_PIN + i, GPIO_OUT );
	}
	gpio_init( MSX_SEL_PIN );
	gpio_set_dir( MSX_SEL_PIN, GPIO_IN );
	gpio_pull_up( MSX_SEL_PIN );
}

// --------------------------------------------------------------------
static uint64_t inline my_get_us( void ) {
	return to_us_since_boot( get_absolute_time() );
}

// --------------------------------------------------------------------
void response_core( void ) {
	static const uint32_t mask = 0x3F << MSX_BUTTON_PIN;
	static const uint64_t sequence_trigger_us = 1100;		//	1100[usec]
	static const uint64_t sequence_finish_us = 1600;		//	1600[usec]
	static uint64_t start_time;
	char s_buffer[256];

	for( ;; ) {
		for( ;; ) {
			//	state 0
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_L ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[1] << MSX_BUTTON_PIN );
			}
			start_time = my_get_us();

			//	state 1
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_H ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[0] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_trigger_us ) {
					break;
				}
			}
			if( (my_get_us() - start_time) > sequence_trigger_us ) {
				break;
			}

			//	state 2
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_L ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[1] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_trigger_us ) {
					break;
				}
			}
			if( (my_get_us() - start_time) > sequence_trigger_us ) {
				break;
			}
			//	state 3
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_H ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[0] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_trigger_us ) {
					break;
				}
			}
			if( (my_get_us() - start_time) > sequence_trigger_us ) {
				break;
			}
			//	state 4
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_L ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[1] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_trigger_us ) {
					break;
				}
			}
			if( (my_get_us() - start_time) > sequence_trigger_us ) {
				break;
			}
			//	state 5
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_H ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[2] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_finish_us ) {
					break;
				}
			}
			if( (my_get_us() - start_time) > sequence_finish_us ) {
				break;
			}
			//	state 6
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_L ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[3] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_finish_us ) {
					break;
				}
			}
			if( (my_get_us() - start_time) > sequence_finish_us ) {
				break;
			}
			//	state 7
			while( gpio_get( MSX_SEL_PIN ) == MSX_SEL_H ) {
				gpio_put_masked( mask, (uint32_t)joymega_matrix[4] << MSX_BUTTON_PIN );
				if( (my_get_us() - start_time) > sequence_finish_us ) {
					break;
				}
			}
		}
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
static void update_matrix( void ) {
	#if DEBUG_UART_ON
		char s_buffer[ 256 ];
	#endif
	static const uint8_t default_matrix[5] = {
		0x33, 0x3F, 0x03, 0x3F, 0x3F
	};
	uint8_t matrix[5];
	uint8_t button;

	//	            b5,b4,b3,b2,b1,b0
	//	matrix[0] = è„ â∫ Çk Çk Ç` Çr
	//	matrix[1] = è„ â∫ ç∂ âE Ça Çb
	//	matrix[2] = Çk Çk Çk Çk Ç` Çr
	//	matrix[3] = Çy Çx Çw Çl Çg Çg
	//	matrix[4] = Çg Çg Çg Çg Ç` Çr

	memcpy( matrix, default_matrix, sizeof(matrix) );
	if( usb_gamepad_report.pos_x <= LEFT_THRESHOLD ) {
		matrix[1] &= 0x37;
	}
	else if( usb_gamepad_report.pos_x >= RIGHT_THRESHOLD ) {
		matrix[1] &= 0x3B;
	}

	if( usb_gamepad_report.pos_y <= TOP_THRESHOLD ) {
		matrix[0] &= 0x1F;
		matrix[1] &= 0x1F;
	}
	else if( usb_gamepad_report.pos_y >= BOTTOM_THRESHOLD ) {
		matrix[0] &= 0x2F;
		matrix[1] &= 0x2F;
	}

	button = usb_gamepad_report.buttons[0];
	if( (button & A_BUTTON) != 0 ) {
		matrix[0] &= 0x3D;
		matrix[2] &= 0x3D;
		matrix[4] &= 0x3D;
	}
	if( (button & B_BUTTON) != 0 ) {
		matrix[1] &= 0x3D;
	}
	if( (button & C_BUTTON) != 0 ) {
		matrix[1] &= 0x3E;
	}
	if( (button & X_BUTTON) != 0 ) {
		matrix[3] &= 0x37;
	}
	if( (button & Y_BUTTON) != 0 ) {
		matrix[3] &= 0x2F;
	}
	if( (button & Z_BUTTON) != 0 ) {
		matrix[3] &= 0x1F;
	}
	if( (button & START_BUTTON) != 0 ) {
		matrix[0] &= 0x3E;
		matrix[2] &= 0x3E;
		matrix[4] &= 0x3E;
	}
	if( (button & MODE_BUTTON) != 0 ) {
		matrix[3] &= 0x3B;
	}
	memcpy( (void*) joymega_matrix, matrix, sizeof(matrix) );

	#if DEBUG_UART_ON
		sprintf( s_buffer, "pos_x = %d\r\n", (int)usb_gamepad_report.pos_x - 128 );
		uart_puts( UART_ID, s_buffer );
		sprintf( s_buffer, "pos_y = %d\r\n", (int)usb_gamepad_report.pos_y - 128 );
		uart_puts( UART_ID, s_buffer );
		sprintf( s_buffer, "buttons1 = 0x%02X\r\n", (int)usb_gamepad_report.buttons[0] );
		uart_puts( UART_ID, s_buffer );
		sprintf( s_buffer, "buttons2 = 0x%02X\r\n", (int)usb_gamepad_report.buttons[1] );
		uart_puts( UART_ID, s_buffer );
	#endif
}

// --------------------------------------------------------------------
void hid_task(void) {
	uint8_t const dev_addr = 1;

	if( !tuh_hid_gamepad_is_mounted( dev_addr ) ) return;

	if( tuh_hid_gamepad_get_report( dev_addr, &usb_gamepad_report ) == TUSB_ERROR_NONE ) {
		update_matrix();
	}
}

// --------------------------------------------------------------------
int main(void) {

	initialization();
	multicore_launch_core1( response_core );

	for( ;; ) {
		tuh_task();
		hid_task();
		led_blinking_task();
	}
	return 0;
}

// --------------------------------------------------------------------
//	ÉQÅ[ÉÄÉpÉbÉhÇ™ê⁄ë±Ç≥ÇÍÇΩÇ∆Ç´Ç…åƒÇ—èoÇ≥ÇÍÇÈÉRÅ[ÉãÉoÉbÉN
//
//	Callback to be called when a gamepad is connected.
//
void tuh_hid_gamepad_mounted_cb(uint8_t dev_addr) {

	tuh_hid_gamepad_get_report( dev_addr, &usb_gamepad_report );
}

// --------------------------------------------------------------------
//	ÉQÅ[ÉÄÉpÉbÉhÇ™êÿífÇ≥ÇÍÇΩÇ∆Ç´Ç…åƒÇ—èoÇ≥ÇÍÇÈÉRÅ[ÉãÉoÉbÉN
//
//	Callback to be called when the gamepad is disconnected.
//
void tuh_hid_gamepad_unmounted_cb( uint8_t dev_addr ) {

	(void) dev_addr;
}
