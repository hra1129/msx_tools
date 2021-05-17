// --------------------------------------------------------------------
//	encode table ‚ğ¶¬‚·‚éƒvƒƒOƒ‰ƒ€
// ====================================================================
//	2021/05/07  HRA! (t.hara)
// --------------------------------------------------------------------

#include <stdio.h>

// --------------------------------------------------------------------
static int encode( int i ) {
	int j, max_j, min_j, count;

	max_j = -1;
	min_j = 12;
	count = 0;
	for( j = 11; j >= 0; j-- ) {
		if( (i & (1 << j)) != 0 ) {
			min_j = j;
			if( max_j == -1 ) {
				max_j = j;
			}
			count++;
		}
	}
	if( count > 2 || count == 0 ) {
		return 15;
	}
	if( max_j == 1 ) {
		return 1;
	}
	if( min_j == 0 ) {
		return 0;
	}
	return max_j;
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	int i;

	printf( "static const int encode_table[] = {\n" );
	for( i = 0; i < (1 << 12); i++ ) {
		if( (i & 15) == 0 ) {
			printf( "\t" );
		}
		printf( "%3d, ", encode(i) );
		if( (i & 15) == 15 ) {
			printf( "\n" );
		}
	}
	printf( "};\n" );

	(void) argc;
	(void) argv;
	return 0;
}
