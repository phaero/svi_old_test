#include "buffer.h"

#include <stdio.h>

#include <seatest.h>

void test_strings_equal( void );
void test_arrays_equal( void );
void test_bits( void );
void test_strings( void );
void test_create_delete_buffer( void );

void setup_create( void );
void teardown_delete( void );
void test_empty_length( void );

void buffer_tests( void )
{
	test_fixture_start();
	run_test(test_strings_equal);
	run_test(test_arrays_equal);
	run_test(test_bits);
	run_test(test_strings);
	run_test(test_create_delete_buffer);
	test_fixture_end();

	test_fixture_start();
	fixture_setup( setup_create );
	fixture_teardown( teardown_delete );
	run_test(test_empty_length);
	test_fixture_end();
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

	// This shouldn't crash
	buffer_del( NULL );
}

struct buffer* test_buffer_ = NULL;
void setup_create( void )
{
	test_buffer_ = buffer_new();
}

void teardown_delete( void )
{
	buffer_del( &test_buffer_ );
}

void test_empty_length( void )
{
	assert_true( buffer_length( test_buffer_ ) == 0 );
}
