#ifndef STEST_STEST_H_INCLUDES
#define STEST_STEST_H_INCLUDES

/*
Assert Declarations/Macros
*/
void s_test_simple_test_result( void* handle, int passed, char* reason, const char* function, unsigned int line);
void s_test_assert_true( void* handle, int test, const char* function, unsigned int line);
void s_test_assert_false( void* handle, int test, const char* function, unsigned int line);
void s_test_assert_int_equal( void* handle, int expected, int actual, const char* function, unsigned int line);
void s_test_assert_float_equal( void* handle, float expected, float actual, float delta, const char* function, unsigned int line);
void s_test_assert_double_equal( void* handle, double expected, double actual, double delta, const char* function, unsigned int line);
void s_test_assert_string_equal( void* handle, char* expected, char* actual, const char* function, unsigned int line);
void s_test_assert_string_ends_with( void* handle, char* expected, char* actual, const char* function, unsigned int line);
void s_test_assert_string_starts_with( void* handle, char* expected, char* actual, const char* function, unsigned int line);
void s_test_assert_string_contains( void* handle, char* expected, char* actual, const char* function, unsigned int line);
void s_test_assert_string_doesnt_contain( void* handle, char* expected, char* actual, const char* function, unsigned int line);

#define assert_true(handle,test) do { \
	s_test_assert_true(handle,test, __func__, __LINE__); \
} while (0)

#define assert_false(handle,test) do { \
	s_test_assert_false(handle,test, __func__, __LINE__); \
} while (0)

#define assert_int_equal(handle,expected, actual) do { \
	s_test_assert_int_equal(handle,expected, actual, __func__, __LINE__); \
} while (0)

#define assert_string_equal(handle,expected, actual) do { \
	s_test_assert_string_equal(handle,expected, actual, __func__, __LINE__); \
} while (0)

#define assert_n_array_equal(handle,expected, actual, n) do { \
	int count; \
	for(count=0; count<n; count++) { \
		char s[100]; \
		sprintf(s,"Expected %d to be %d at position %d", actual[count], expected[count], count); \
		s_test_simple_test_result(handle,(expected[count] == actual[count]), s, __func__, __LINE__); \
	} \
} while (0)

#define assert_bit_set(handle,bit_number, value) do { \
	s_test_simple_test_result(handle,((1 << bit_number) & value), " Expected bit to be set" , __func__, __LINE__); \
} while (0)

#define assert_bit_not_set(handle,bit_number, value) do { \
	s_test_simple_test_result(handle,!((1 << bit_number) & value), " Expected bit not to to be set" ,  __func__, __LINE__); \
} while (0)

#define assert_bit_mask_matches(handle,value, mask) do { \
	s_test_simple_test_result(handle,((value & mask) == mask), " Expected all bits of mask to be set" ,  __func__, __LINE__); \
} while (0)

#define assert_fail(handle,message) do { \
	s_test_simple_test_result(handle,0, message,  __func__, __LINE__); \
} while (0)

#define assert_float_equal(handle,expected, actual, delta) do { \
	s_test_assert_float_equal(handle,expected, actual, delta, __func__, __LINE__); \
} while (0)

#define assert_double_equal(handle,expected, actual, delta) do { \
	s_test_assert_double_equal(handle,expected, actual, delta, __func__, __LINE__); \
} while (0)

#define assert_string_contains(handle,expected, actual) do { \
	s_test_assert_string_contains(handle,expected, actual, __func__, __LINE__); \
} while (0)

#define assert_string_doesnt_contain(handle,expected, actual) do { \
	s_test_assert_string_doesnt_contain(handle,expected, actual, __func__, __LINE__); \
} while (0)

#define assert_string_starts_with(handle,expected, actual) do { \
	s_test_assert_string_starts_with(handle,expected, actual, __func__, __LINE__); \
} while (0)

#define assert_string_ends_with(handle,expected, actual) do { \
	s_test_assert_string_ends_with(handle,expected, actual, __func__, __LINE__); \
} while (0)

/*
Fixture / Test Management
#define NUMARGS(...)  (sizeof((s_test_register_fp[]){__VA_ARGS__})/sizeof(s_test_register_fp))

#define s_setup(name) static void s_test_setup_##name( void )

#define s_teardown(name) static void s_test_teardown_##name( void )

#define s_test(name) static void s_test_##name( void ); \
static void ##name( uint64_t* handle ) { \
	s_internal_test( #name, handle, NULL, s_test_##name, NULL ); \
} \
static void s_test_##name( void ) \

#define s_test_f(name,setup,teardown) static void s_test_##name( void ); \
static void ##name( uint64_t* handle ) { \
	s_internal_test( #name, handle, s_test_setup_##setup, s_test_##name, s_test_teardown_##teardown ); \
} \
static void s_test_##name( void )

#define s_def_test_group(name) void s_test_group_##name( uint64_t* handle )

#define s_test_group(name,...) void s_test_group_##name( uint64_t* handle ) { \
	s_internal_test_group( #name, handle, NUMARGS(__VA_ARGS__), __VA_ARGS__ ); \
}

#define s_test_groups(...) int main( int argc, char* argv[] ) { \
	return s_internal_main( argc, argv, NUMARGS(__VA_ARGS__), __VA_ARGS__ ); \
}
*/

typedef void (*s_test_fp)( void* handle );
void s_test_init( void** handle );
void* s_test_get_data( void* handle );
void s_test_set_data( void* handle, void* data );
void s_test_add_test( void* handle, const char* test_name, s_test_fp test_func );
void s_test_add_test_f( void* handle, const char* test_name,
		s_test_fp test_func, s_test_fp test_setup, s_test_fp test_teardown );
void s_test_add_group( void* handle, const char* group_name, s_test_fp test_group );
int s_test_main( int argc, const char* argv[], void* handle );

#endif
