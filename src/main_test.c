#include <seatest.h>

void buffer_tests( void );

void all_tests( void )
{
	buffer_tests();
}

int main( int argc, char* argv[] ) {
	return run_tests( all_tests );
}

