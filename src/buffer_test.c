#include "buffer.h"

#include <stdio.h>

#include <stest.h>

void test_strings_equal( void* handle );
void test_arrays_equal( void* handle );
void test_bits( void* handle );
void test_strings( void* handle );
void test_create_delete_buffer( void* handle );
void test_delete_invalid_buffer( void* handle );

void setup_create( void* handle );
void teardown_delete( void* handle );
void test_empty_length( void* handle );

void buffer_tests( void* handle ) {
	s_test_add_test( handle, "strings_equal", test_strings_equal);
	s_test_add_test( handle, "arrays_equal", test_arrays_equal);
	s_test_add_test( handle, "bits", test_bits);
	s_test_add_test( handle, "strings", test_strings);
	s_test_add_test( handle, "create_delete_buffer", test_create_delete_buffer);
	s_test_add_test( handle, "delete_invalid_buffer", test_delete_invalid_buffer);

	s_test_add_test_f( handle, "empty_length", test_empty_length, setup_create, teardown_delete );
}

void test_strings_equal( void* handle )
{
	char *s = "hello";
	assert_string_equal( handle, "hello", s);
}

void test_arrays_equal( void* handle )
{
	unsigned char expected_bytes[] = { 1, 2, 3};
	unsigned char buffer[5];
	int i;

	// put 5 bytes in
	for(i=0; i<5; i++) {
		buffer[i]=i+1;
	}

	// only check the first 3
	assert_n_array_equal( handle, expected_bytes, buffer, 3);
}

void test_bits( void* handle )
{
	assert_bit_set( handle, 0, 0x01);
	assert_bit_set( handle, 2, 0x04);
	assert_bit_not_set( handle, 3, 0x02);
}

void test_strings( void* handle )
{
	char *s = "hello";
	assert_string_equal( handle, "hello", s);
	assert_string_contains( handle, "blah", "why say blah?");
	assert_string_doesnt_contain( handle, "blah", "why say hello?");
	assert_string_ends_with( handle, "h?", "why say blah?");
	assert_string_starts_with( handle, "why", "why say blah?");
}

void test_create_delete_buffer( void* handle )
{
	struct buffer* buf = NULL;

	buf = buffer_new();
	assert_true(  handle, buf != NULL );

	buffer_del( &buf );
	assert_true(  handle, buf == NULL );
}

void test_delete_invalid_buffer( void* handle )
{
	struct buffer* buf = NULL;

	// This shouldn't crash
	buffer_del( NULL );
	buffer_del( &buf );
}

struct buffer* test_buffer_ = NULL;
void setup_create( void* handle )
{
	test_buffer_ = buffer_new();
}

void teardown_delete( void* handle )
{
	buffer_del( &test_buffer_ );
}

void test_empty_length( void* handle )
{
	assert_true( handle, buffer_length( test_buffer_ ) == 0 );
}
