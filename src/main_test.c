#include <seatest.h>

void svi_buf_tests( void );

void all_tests( void )
{
	svi_buf_tests();
}

int main( int argc, char* argv[] ) {
	return run_tests( all_tests );
}

