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
