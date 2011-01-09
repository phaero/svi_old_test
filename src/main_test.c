#include <seatest.h>

/**
 * Declare the test registration functions here to avoid a extra header file for each test file.
 */

void buffer_tests( void );

/**
 * Run all test suites.
 *
 * This will insert a main method!
 */
test_suites(
		buffer_tests
	)
