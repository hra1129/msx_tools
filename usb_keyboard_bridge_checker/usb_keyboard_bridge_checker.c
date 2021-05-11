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

#define MY_BIT( n )		( 1 << (n) )

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

//	MSX_KEYMATRIX_INV
//		0: Y11-Y0 は正論理である
//		1: Y11-Y0 は負論理である
//
//		0: Y11-Y0 is positive logic.
//		1: Y11-Y0 is negative logic.
//
#define MSX_KEYMATRIX_INV 1

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
#define Y0_PIN  16
#define Y1_PIN  17
#define Y2_PIN  18
#define Y3_PIN  19
#define Y4_PIN  20
#define Y5_PIN  21
#define Y6_PIN  22
#define Y7_PIN  15
#define Y8_PIN  14
#define Y9_PIN  13
#define Y10_PIN 12
#define Y11_PIN 11

#define X0_PIN     10
#define X1_PIN      9
#define X2_PIN      8
#define X3_PIN      7
#define X4_PIN      6
#define X5_PIN      5
#define X6_PIN      4
#define X7_PIN      3
#define PAUSE_PIN   2

#if MSX_KEYMATRIX_ROW_TYPE == 2
	static const uint32_t y_mask = 
		MY_BIT(X0_PIN ) | 
		MY_BIT(X1_PIN ) | 
		MY_BIT(X2_PIN ) | 
		MY_BIT(X3_PIN ) | 
		MY_BIT(Y0_PIN );

	static const uint32_t xy_mask = 
		MY_BIT(X0_PIN ) | 
		MY_BIT(X1_PIN ) | 
		MY_BIT(X2_PIN ) | 
		MY_BIT(X3_PIN );
#else
	static const uint32_t y_mask = 
		MY_BIT(Y0_PIN ) | 
		MY_BIT(Y1_PIN ) | 
		MY_BIT(Y2_PIN ) | 
		MY_BIT(Y3_PIN ) | 
		MY_BIT(Y4_PIN ) | 
		MY_BIT(Y5_PIN ) | 
		MY_BIT(Y6_PIN ) | 
		MY_BIT(Y7_PIN ) | 
		MY_BIT(Y8_PIN ) | 
		MY_BIT(Y9_PIN ) | 
		MY_BIT(Y10_PIN) | 
		MY_BIT(Y11_PIN);
#endif

static const uint32_t x_mask = 
	MY_BIT(X0_PIN   ) |
	MY_BIT(X1_PIN   ) |
	MY_BIT(X2_PIN   ) |
	MY_BIT(X3_PIN   ) |
	MY_BIT(X4_PIN   ) |
	MY_BIT(X5_PIN   ) |
	MY_BIT(X6_PIN   ) |
	MY_BIT(X7_PIN   ) |
	MY_BIT(PAUSE_PIN);

static const uint32_t y_bit[] = {
	MY_BIT(Y0_PIN ),
	MY_BIT(Y1_PIN ),
	MY_BIT(Y2_PIN ),
	MY_BIT(Y3_PIN ),
	MY_BIT(Y4_PIN ),
	MY_BIT(Y5_PIN ),
	MY_BIT(Y6_PIN ),
	MY_BIT(Y7_PIN ),
	MY_BIT(Y8_PIN ),
	MY_BIT(Y9_PIN ),
	MY_BIT(Y10_PIN),
	MY_BIT(Y11_PIN),
};

static const uint32_t x_bit[] = {
	MY_BIT(X0_PIN   ),
	MY_BIT(X1_PIN   ),
	MY_BIT(X2_PIN   ),
	MY_BIT(X3_PIN   ),
	MY_BIT(X4_PIN   ),
	MY_BIT(X5_PIN   ),
	MY_BIT(X6_PIN   ),
	MY_BIT(X7_PIN   ),
	MY_BIT(PAUSE_PIN),
};

static const uint32_t y_bit_num[] = {
	Y0_PIN ,
	Y1_PIN ,
	Y2_PIN ,
	Y3_PIN ,
	Y4_PIN ,
	Y5_PIN ,
	Y6_PIN ,
	Y7_PIN ,
	Y8_PIN ,
	Y9_PIN ,
	Y10_PIN,
	Y11_PIN,
};

static const uint8_t x_bit_num[] = {
	X0_PIN   ,
	X1_PIN   ,
	X2_PIN   ,
	X3_PIN   ,
	X4_PIN   ,
	X5_PIN   ,
	X6_PIN   ,
	X7_PIN   ,
	PAUSE_PIN,
};

// --------------------------------------------------------------------
#if MSX_KEYMATRIX_ROW_TYPE == 0
	static uint32_t inline get_output_y_value( int y ) {
		#if MSX_KEYMATRIX_INV == 0
			return y << Y0_PIN;
		#else
			return ~(y << Y0_PIN);
		#endif
	}
#elif MSX_KEYMATRIX_ROW_TYPE == 1
	static uint32_t inline get_output_y_value( int y ) {
		#if MSX_KEYMATRIX_INV == 0
			return y_bit[ y ];
		#else
			return ~y_bit[ y ];
		#endif
	}
#else
	static uint32_t inline get_output_y_value( int y ) {
		#if MSX_KEYMATRIX_INV == 0
			return (y << X0_PIN) | MY_BIT( Y0_PIN );
		#else
			return ~(y << X0_PIN);
		#endif
	}
#endif

// --------------------------------------------------------------------
void initializer( void ) {
	int i;
    stdio_init_all();

	for( i = 0; i < 12; i++ ) {		// Y0-Y11, 12bits
		gpio_init( y_bit_num[i] );
		gpio_set_dir( y_bit_num[i], GPIO_OUT );
		#if MSX_KEYMATRIX_ROW_PULL_UP == 1
			gpio_pull_up( y_bit_num[i] );
		#elif MSX_KEYMATRIX_ROW_PULL_UP == 2
			gpio_pull_down( y_bit_num[i] );
		#endif
	}

	for( i = 0; i < 9; i++ ) {		// X0-X7 and PAUSE, 9bits
		gpio_init( x_bit_num[i] );
		gpio_set_dir( x_bit_num[i], GPIO_IN );
		#if MSX_KEYMATRIX_ROW_PULL_UP == 1
			gpio_pull_up( x_bit_num[i] );
		#elif MSX_KEYMATRIX_ROW_PULL_UP == 2
			gpio_pull_down( x_bit_num[i] );
		#endif
	}
}

// --------------------------------------------------------------------
int main() {
	int i, j;
	uint32_t y;
	uint32_t matrix[ 13 ];

	initializer();

	for( ;; ) {
		for( i = 0; i < 12; i++ ) {
			//	Yに行番号を出力
			y = get_output_y_value(i);
			#if MSX_KEYMATRIX_ROW_TYPE == 2
				//	X0-X3 を出力に変更
				gpio_set_dir_out_masked( xy_mask );
			#endif
			gpio_put_masked( y_mask, y );

			//	反映されるのを待つ
			sleep_us( 2 );

			#if MSX_KEYMATRIX_ROW_TYPE == 2
				//	X0-X3 を入力に変更して、DIR信号を L に落とす
				gpio_set_dir_in_masked( xy_mask );
				gpio_put( Y0_PIN, false );
				//	反映されるのを待つ
				sleep_us( 1 );
			#endif

			//	Xを読み取る
			matrix[i] = gpio_get_all();
		}
		matrix[12] = gpio_get( PAUSE_PIN );

		//	表示
		printf( "\x1B" "7Y : 76543210\r\n" );
		for( i = 0; i < 12; i++ ) {
			printf( "%2d: ", i );
			for( j = 7; j >= 0; j-- ) {
				printf( "%c", ((matrix[i] & x_bit[j]) != 0) ? '1' : '0' );
			}
			printf( "\r\n" );
		}
		printf( "\x1B" "8" );
		sleep_ms( 500 );
	}
	return 0;
}
