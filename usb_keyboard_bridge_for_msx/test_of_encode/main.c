// --------------------------------------------------------------------
//	encode()関数のテスト
// ====================================================================
//	2021/5/4	t.hara
// --------------------------------------------------------------------

#include <stdio.h>

// --------------------------------------------------------------------
//	ここに encode() をコピペ
	static int inline encode( int y ) {
		int i;

		for( i = 0; i < 12; i++ ) {
			if( y == (1 << i) ) {
				return i;
			}
		}
		return 15;
	}

// --------------------------------------------------------------------
static void test_item( input_y, reference_result ) {
	int y;

	y = encode( input_y, reference_result );
	if( y == reference_result ) {
		printf( "test_item( 0x%04X, 0x%04X ) : PASSED\n", input_y, reference_result );
	}
	else {
		printf( "test_item( 0x%04X, 0x%04X ) : FAILED (0x%04X)\n", input_y, reference_result, y );
	}
}

// --------------------------------------------------------------------
int main( int argc, char argv[] ) {

	test_item( 0x001, 0x00 );
	test_item( 0x002, 0x01 );
	test_item( 0x004, 0x02 );
	test_item( 0x008, 0x03 );
	test_item( 0x010, 0x04 );
	test_item( 0x020, 0x05 );
	test_item( 0x040, 0x06 );
	test_item( 0x080, 0x07 );
	test_item( 0x100, 0x08 );
	test_item( 0x200, 0x09 );
	test_item( 0x400, 0x0A );
	test_item( 0x800, 0x0B );

	test_item( 0x000, 0x0F );
	test_item( 0xFFF, 0x0F );
	test_item( 0xAAA, 0x0F );
	test_item( 0x555, 0x0F );
	test_item( 0x123, 0x0F );

	(void) argc;
	(void) argv;
	return 0;
}
