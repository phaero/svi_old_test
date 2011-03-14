#ifndef SEATEST_SEATEST_H_INCLUDES
#define SEATEST_SEATEST_H_INCLUDES

/*
Declarations
*/
typedef void (*seatest_test_setup_fp)( void** data );
typedef void (*seatest_test_test_fp)( void* data );
typedef void (*seatest_test_teardown_fp)( void *data );

void seatest_register_test_fixture(
		const char* fname,
		const char* test_name,
		seatest_test_setup_fp* setup,
		seatest_test_test_fp* test,
		seatest_test_teardown_fp* teardown
	);

#define reg_test(test) do { \
	seatest_register_test_fixture( __FILE__, #test, NULL, test, NULL ); \
} while (0)

#define reg_test_f(setup,test,teardown) do { \
	seatest_register_test_fixture( __FILE__, #test, setup, test, teardown ); \
} while (0)

void seatest_test_fixture_start(char* filepath);
void seatest_test_fixture_end( void );
void seatest_simple_test_result(int passed, char* reason, const char* function, unsigned int line);
int seatest_should_run( char* fixture, char* test);
void seatest_run_test(void);
void seatest_setup();
void seatest_teardown();

/*
Assert Declarations/Macros
*/
void seatest_assert_true(int test, const char* function, unsigned int line);
void seatest_assert_false(int test, const char* function, unsigned int line);
void seatest_assert_int_equal(int expected, int actual, const char* function, unsigned int line);
void seatest_assert_float_equal(float expected, float actual, float delta, const char* function, unsigned int line);
void seatest_assert_double_equal(double expected, double actual, double delta, const char* function, unsigned int line);
void seatest_assert_string_equal(char* expected, char* actual, const char* function, unsigned int line);
void seatest_assert_string_ends_with(char* expected, char* actual, const char* function, unsigned int line);
void seatest_assert_string_starts_with(char* expected, char* actual, const char* function, unsigned int line);
void seatest_assert_string_contains(char* expected, char* actual, const char* function, unsigned int line);
void seatest_assert_string_doesnt_contain(char* expected, char* actual, const char* function, unsigned int line);

#define assert_true(test) do { \
	seatest_assert_true(test, __FUNCTION__, __LINE__); \
} while (0)

#define assert_false(test) do { \
	seatest_assert_false(test, __FUNCTION__, __LINE__); \
} while (0)

#define assert_int_equal(expected, actual) do { \
	seatest_assert_int_equal(expected, actual, __FUNCTION__, __LINE__); \
} while (0)

#define assert_string_equal(expected, actual) do { \
	seatest_assert_string_equal(expected, actual, __FUNCTION__, __LINE__); \
} while (0)

#define assert_n_array_equal(expected, actual, n) do { \
	int count; \
	for(count=0; count<n; count++) { \
		char s[100]; \
		sprintf(s,"Expected %d to be %d at position %d", actual[count], expected[count], count); \
		seatest_simple_test_result((expected[count] == actual[count]), s, __FUNCTION__, __LINE__); \
	} \
} while (0)

#define assert_bit_set(bit_number, value) do { \
	seatest_simple_test_result(((1 << bit_number) & value), " Expected bit to be set" , __FUNCTION__, __LINE__); \
} while (0)

#define assert_bit_not_set(bit_number, value) do { \
	seatest_simple_test_result(!((1 << bit_number) & value), " Expected bit not to to be set" ,  __FUNCTION__, __LINE__); \
} while (0)

#define assert_bit_mask_matches(value, mask) do { \
	seatest_simple_test_result(((value & mask) == mask), " Expected all bits of mask to be set" ,  __FUNCTION__, __LINE__); \
} while (0)

#define assert_fail(message) do { \
	seatest_simple_test_result(0, message,  __FUNCTION__, __LINE__); \
} while (0)

#define assert_float_equal(expected, actual, delta) do { \
	seatest_assert_float_equal(expected, actual, delta, __FUNCTION__, __LINE__); \
} while (0)

#define assert_double_equal(expected, actual, delta) do { \
	seatest_assert_double_equal(expected, actual, delta, __FUNCTION__, __LINE__); \
} while (0)

#define assert_string_contains(expected, actual) do { \
	seatest_assert_string_contains(expected, actual, __FUNCTION__, __LINE__); \
} while (0)

#define assert_string_doesnt_contain(expected, actual) do { \
	seatest_assert_string_doesnt_contain(expected, actual, __FUNCTION__, __LINE__); \
} while (0)

#define assert_string_starts_with(expected, actual) do { \
	seatest_assert_string_starts_with(expected, actual, __FUNCTION__, __LINE__); \
} while (0)

#define assert_string_ends_with(expected, actual) do { \
	seatest_assert_string_ends_with(expected, actual, __FUNCTION__, __LINE__); \
} while (0)

/*
Fixture / Test Management
*/

void fixture_setup(void (*setup)( void ));
void fixture_teardown(void (*teardown)( void ));

#define run_test(test) do { \
	if(seatest_should_run(__FILE__, #test)) {\
		seatest_setup(); \
		test(); \
		seatest_teardown(); \
		seatest_run_test(); \
	} \
} while (0)

#define test_fixture_start() do { \
	seatest_test_fixture_start(__FILE__); \
} while (0)

#define test_fixture_end() do { \
	seatest_test_fixture_end(); \
} while (0)

void fixture_filter(char* filter);
void test_filter(char* filter);

typedef void (*seatest_test_suite_fp)( void );
typedef void (*test_suite_fp)( void );
int seatest_internal_main( int argc, char* argv[], int num, ... );
#define NUMARGS(...)  (sizeof((seatest_test_suite_fp[]){__VA_ARGS__})/sizeof(seatest_test_suite_fp))
#define test_suites(...) int main( int argc, char* argv[] ) { \
	return seatest_internal_main( argc, argv, NUMARGS(__VA_ARGS__), __VA_ARGS__ ); \
}

#define s_setup(name) static void s_setup_##name( void )

#define s_teardown(name) static void s_teardown_##name( void )

#define s_test(name) static void s_test_##name( void )

#define s_test_f(name,setup,teardown) static void s_test_##name( void ) { \
	s_setup_##setup(); \
	s_test_f_##name(); \
	s_teardown_##teardown(); \
} \
static void s_test_f_##name( void )

#define s_def_test_group(name) void s_test_group_##name( void )

#define s_test_group(name,...) void s_test_group_##name( void ) { \
	s_internal_test_group( ##name, NUMARGS(__VA_ARGS__), __VA_ARGS__ ); \
}

#define s_test_groups(...) int main( int argc, char* argv[] ) { \
	return s_internal_main( argc, argv, NUMARGS(__VA_ARGS__), __VA_ARGS__ ); \
}

#endif
