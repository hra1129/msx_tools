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
#include "pico/stdlib.h"

// --------------------------------------------------------------------
//	MSX_KEYMATRIX_ROW_TYPE
//		0: Y3-Y0  �� ROW�A�h���X���w�肳���
//		1: Y11-Y0 �� ROW�A�h���X���f�R�[�h�������ʂ��w�肳��� (ONE HOT)
//
#define MSX_KEYMATRIX_ROW_TYPE 1

//	MSX_KEYMATRIX_INV
//		0: Y11-Y0 �͐��_���ł���
//		1: Y11-Y0 �͕��_���ł���
//
#define MSX_KEYMATRIX_INV 1

//	MSX_KEYMATRIX_ROW_PULL_UP
//		0: Y0-Y11 �̓��̓s���� ���� PULL UP/DOWN �͎g��Ȃ�
//		1: Y0-Y11 �̓��̓s���� ���� PULL UP ���g��
//		2: Y0-Y11 �̓��̓s���� ���� PULL DOWN ���g��
//
#define MSX_KEYMATRIX_ROW_PULL_UP 1

//	MSX_KEYMATRIX_ROW_PIN
//		Y0 �� GPIO�ԍ��BY1-Y11 �́A��������C���N�������g���邩�����̘A��
//
#define MSX_KEYMATRIX_ROW_PIN 2

//	MSX_KEYMATRIX_RESULT_PIN
//		X0 �� GPIO�ԍ��BX1-X7 �́A��������C���N�������g����`�̘A��
//
#define MSX_KEYMATRIX_RESULT_PIN 14

// --------------------------------------------------------------------
void initializer( void ) {
	int i;
    stdio_init_all();

	//	GPIO�̐M���̌�����ݒ�
	for( i = 0; i < 12; i++ ) {		// Y0-Y11, 12bits
		gpio_set_dir( MSX_KEYMATRIX_ROW_PIN + i, GPIO_OUT );
	}
	for( i = 0; i < 9; i++ ) {		// X0-X7 and PAUSE, 9bits
		gpio_set_dir( MSX_KEYMATRIX_RESULT_PIN + i, GPIO_IN );
		#if MSX_KEYMATRIX_ROW_PULL_UP == 1
			gpio_pull_up( MSX_KEYMATRIX_ROW_PIN + i );
		#elif MSX_KEYMATRIX_ROW_PULL_UP == 2
			gpio_pull_down( MSX_KEYMATRIX_ROW_PIN + i );
		#endif
	}
}

// --------------------------------------------------------------------
int main() {
	int i;
	uint32_t y;
	static const uint32_t y_mask = 0x0FFF << MSX_KEYMATRIX_ROW_PIN;
	uint8_t matrix[ 13 ];

	initializer();

	for( ;; ) {
		for( i = 0; i < 12; i++ ) {
			//	Y�����
			#if MSX_KEYMATRIX_ROW_TYPE == 0
				y = i;
			#else
				y = 1 << i;
			#endif

			#if MSX_KEYMATRIX_INV == 1
				y = (gpio_get_all() >> MSX_KEYMATRIX_ROW_PIN) ^ 0x0FFF;
			#endif
			gpio_put_masked( y_mask, y );

			//	���f�����̂�҂�
			sleep_us( 2 );

			//	X��ǂݎ��
			matrix[i] = (gpio_get_all() >> MSX_KEYMATRIX_RESULT_PIN) & 0xFF;
		}
		matrix[12] = gpio_get( MSX_KEYMATRIX_RESULT_PIN + 8 ) | 0xFE;

		//	�\��
		for( i = 0; i < 13; i++ ) {
			printf( "%2d: %02X\r\n", i, (int)matrix[i] );
		}
		sleep_ms( 500 );
	}
	return 0;
}
