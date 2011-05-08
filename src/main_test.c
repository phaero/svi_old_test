#include <stdio.h>

#include <stest.h>

/**
 * Declare the test group functions here to avoid a extra header file for each test file.
 */
extern void buffer_tests( void* handle );

int main( int argc, const char* argv[] ) {
	void* handle = NULL;
	
	s_test_init( &handle );
	s_test_add_group( handle, "buffer_tests", buffer_tests );

	return s_test_main( argc, argv, handle );
}
