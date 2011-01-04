#include "buffer.h"

#include <stdio.h>

#include <seatest.h>

void test_strings_equal( void );
void test_arrays_equal( void );
void test_bits( void );
void test_strings( void );
void test_create_delete_buffer( void );

void buffer_tests( void )
{
	test_fixture_start();               // starts a fixture
	run_test(test_strings_equal);
	run_test(test_arrays_equal);
	run_test(test_bits);
	run_test(test_strings);   // run tests
	run_test(test_create_delete_buffer);
	test_fixture_end();                 // ends a fixture
}

void test_strings_equal( void )
{
	char *s = "hello";
	assert_string_equal("hello", s);
}

void test_arrays_equal( void )
{
	unsigned char expected_bytes[] = { 1, 2, 3};
	unsigned char buffer[5];
	int i;

	// put 5 bytes in
	for(i=0; i<5; i++) buffer[i]=i+1;

	// only check the first 3
	assert_n_array_equal(expected_bytes, buffer, 3);
}

void test_bits( void )
{
	assert_bit_set(0, 0x01);
	assert_bit_set(2, 0x04);
	assert_bit_not_set(3, 0x02);    
}

void test_strings( void )
{
	char *s = "hello";
	assert_string_equal("hello", s);
	assert_string_contains("blah", "why say blah?");
	assert_string_doesnt_contain("blah", "why say hello?");
	assert_string_ends_with("h?", "why say blah?");
	assert_string_starts_with("why", "why say blah?");
}

void test_create_delete_buffer( void )
{
	struct buffer* buf = NULL;

	buf = buffer_new();
	assert_true( buf != NULL );

	buffer_del( &buf );
	assert_true( buf == NULL );
}
