s_setup(setup) {
	s_set_data(xx);
}

s_teardown(teardown) {
	void* data = s_get_data();
	free( data );
}

s_test(simple) {
	// XXX
}

s_test_f(advanced,setup,teardown) {
	void* data = s_get_data();
}

// Needs to be before s_test_group if in the same file otherwise before s_test_groups ( need one for each test_group )
s_def_test_group( buffer_tests );

// Test group def, needs to be last in a file unless test groups are present
s_test_group(
		// Test group name, have it as a string instead to make it more clear?
		buffer_tests, 
		simple,
		advanced
	) // no semicolon

// list of all test groups, doesn't need to be in the same file
s_test_groups(
		buffer_tests
	) // no semicolon

// Alternative 2
// 
void test_func( void* handle ) {
	// Do Tests
}

void test_setup( void* handle ) {
	void* data = malloc();
	s_test_set_data( handle, data );
}
void test_func_f( void* handle ) {
	void* data = s_test_get_data( handle );
	// Do Tests
}
void test_teardown( void* handle ) {
	void* data = s_test_get_data( handle );
	free( data );
}

void test_group( void* handle ) {
	s_test_add_test( handle, "Test name", test_func );
	s_test_add_test_f( handle, "Test name", test_func_f, test_setup, test_teardown );

	return;
}

int main( int argc, const char* argv[] ) {
	s_test_init( handle );
	s_test_add_group( handle, "Group name", test_group_fp );

	return s_test_main( argc, argv, handle );
}
