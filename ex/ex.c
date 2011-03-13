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

// Possible to generate?
s_test_group(
		buffer_tests, // get from filename?
		simple,
		advanced
	)

// Possible to generate?
s_test_groups(
		buffer_tests
	)
