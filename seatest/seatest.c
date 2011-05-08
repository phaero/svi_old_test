#include "seatest.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include <sys/time.h>

#include <glib.h>
#include <glib/gprintf.h>

struct STest {
	GSList* groups;
};

struct TestGroup {
	gchar* name;
	GSList* tests;
};

struct TestResult {
	bool passed;
	GSList* msgs;

	uint64_t start;
	uint64_t end;
};

struct Test {
	gchar* name;

	s_test_fp setup;
	s_test_fp test;
	s_test_fp teardown;

	void* data;

	struct TestResult result;
};

char* test_file_name(char* path)
{
	char* file = path + strlen(path);

	while (file != path && *file!= '\\' ) {
		file--;
	}

	if (*file == '\\') {
		file++;
	}

	return file;
}

void seatest_simple_test_result( void* handle, int passed, char* reason, const char* function, unsigned int line)
{
	struct Test* test = (struct Test*)handle;
	if (!passed) {
		gchar msg_[255];
		gint size_ = 0;

		size_ = g_snprintf( msg_, 255, "%-20s Line %-5d %s\r\n", (char*)function, line, reason );

		test->result.passed = false;
		test->result.msgs = g_slist_append( test->result.msgs, g_strndup( msg_, size_ ) );
	} else {
		test->result.passed = test->result.passed ? true : false;
	}
}

void seatest_assert_true( void* handle, int test, const char* function, unsigned int line)
{
	seatest_simple_test_result( handle, test, "Should of been true", function, line);
}

void seatest_assert_false( void* handle, int test, const char* function, unsigned int line)
{
	seatest_simple_test_result( handle, !test, "Should of been false", function, line);
}


void seatest_assert_int_equal( void* handle, int expected, int actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %d but was %d", expected, actual);
	seatest_simple_test_result( handle,expected==actual, s, function, line);
}

void seatest_assert_float_equal( void* handle, float expected, float actual, float delta, const char* function, unsigned int line )
{
	char s[100];
	float result = expected-actual;
	sprintf(s, "Expected %f but was %f", expected, actual);

	if (result < 0.0) {
		result = 0.0 - result;
	}

	seatest_simple_test_result( handle, result <= delta, s, function, line);
}

void seatest_assert_double_equal( void* handle, double expected, double actual, double delta, const char* function, unsigned int line )
{
	char s[100];
	double result = expected-actual;
	sprintf(s, "Expected %f but was %f", expected, actual);

	if (result < 0.0) {
		result = 0.0 - result;
	}

	seatest_simple_test_result( handle, result <= delta, s, function, line);
}

void seatest_assert_string_equal(void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s but was %s", expected, actual);
	seatest_simple_test_result(handle, strcmp(expected, actual)==0, s, function, line);
}

void seatest_assert_string_ends_with( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s to end with %s", actual, expected);
	seatest_simple_test_result( handle, strcmp(expected, actual+(strlen(actual)-strlen(expected)))==0, s, function, line);
}

void seatest_assert_string_starts_with( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s to start with %s", actual, expected);
	seatest_simple_test_result( handle, strncmp(expected, actual, strlen(expected))==0, s, function, line);
}

void seatest_assert_string_contains( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s to be in %s", expected, actual);
	seatest_simple_test_result( handle, strstr(actual, expected)!=NULL, s, function, line);
}

void seatest_assert_string_doesnt_contain( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s not to have %s in it", actual, expected);
	seatest_simple_test_result(handle, strstr(actual, expected)==NULL, s, function, line);
}

/*
void seatest_header_printer(char* s, int length, char f)
{
	int l = strlen(s);
	int d = (length- (l + 2)) / 2;
	int i;

	for (i = 0; i<d; i++) {
		printf("%c",f);
	}

	printf(" %s ", s);

	for (i = (d+l+2); i<length; i++) {
		printf("%c",f);
	}

	printf("\r\n");
}


void seatest_test_fixture_start(char* filepath)
{
	seatest_current_fixture = test_file_name(filepath);
	seatest_header_printer(seatest_current_fixture, 50, '-');
	seatest_fixture_tests_failed = sea_tests_failed;
	seatest_fixture_tests_run = sea_tests_run;
	seatest_fixture_teardown = 0;
	seatest_fixture_setup = 0;
}

void seatest_test_fixture_end()
{
	char s[100];
	sprintf(s, "%d run  %d failed", sea_tests_run-seatest_fixture_tests_run, sea_tests_failed-seatest_fixture_tests_failed);
	seatest_header_printer(s, 50, ' ');
	printf("\r\n");
}

static char* seatest_fixture_filter = 0;
static char* seatest_test_filter = 0;

void fixture_filter(char* filter)
{
	seatest_fixture_filter = filter;
}


void test_filter(char* filter)
{
	seatest_test_filter = filter;
}


int seatest_should_run( char* fixture, char* test)
{
	int run = 1;
	if (seatest_fixture_filter) {
		if (strncmp(seatest_fixture_filter, fixture, strlen(seatest_fixture_filter)) != 0) run = 0;
	}
	if (seatest_test_filter) {
		if (strncmp(seatest_test_filter, test, strlen(seatest_test_filter)) != 0) run = 0;
	}
	return run;
}
*/

int run_tests( s_test_fp tests )
{
	uint64_t start;
	uint64_t end;
	struct timeval tv;

	gettimeofday( &tv, NULL );
	start = tv.tv_sec * 1000000 + tv.tv_usec;

	// Run tests
	/*tests();*/

	gettimeofday( &tv, NULL );
	end = tv.tv_sec * 1000000 + tv.tv_usec;

	/*
	printf("\r\n\r\n==================================================\r\n");
	if (sea_tests_failed > 0) {
		printf("                      Failed\r\n");
	} else {
		printf("               ALL TESTS PASSED\r\n");
	}
	printf("                 %d tests run\r\n", sea_tests_run);
	printf("                    in %" PRIu64 " ms\r\n", end - start );
	printf("==================================================\r\n");

	return sea_tests_failed == 0;
	*/
	return 0;
}

void s_test_init( void** handle ) {
	struct STest* stest = g_new( struct STest, 1 );
	*handle = stest;
}

void* s_test_get_data( void* handle ) {
	struct Test* test = (struct Test*)handle;
	return test->data;
}

void s_test_set_data( void* handle, void* data ) {
	/*TODO Add warning if data is not deallocated in teardown*/
	struct Test* test = (struct Test*)handle;
	test->data = data;
}

void s_test_add_test( void* handle, const char* test_name, s_test_fp test_func ) {
	struct TestGroup* group = (struct TestGroup*)handle;

	/*Create test struct*/
	struct Test* test = g_new( struct Test, 1 );
	test->name = g_strndup( test_name, strlen( test_name ) );
	test->setup = NULL;
	test->test = test_func;
	test->teardown = NULL;
	test->data = NULL;
	test->result.passed = true;
	test->result.msgs = NULL;
	test->result.start = 0;
	test->result.end = 0;

	group->tests = g_slist_append( group->tests, test );
}

void s_test_add_test_f( void* handle, const char* test_name,
		s_test_fp test_func, s_test_fp test_setup, s_test_fp test_teardown ) {
	struct TestGroup* group = (struct TestGroup*)handle;

	/*Create test struct*/
	struct Test* test = g_new( struct Test, 1 );
	test->name = g_strndup( test_name, strlen( test_name ) );
	test->setup = test_setup;
	test->test = test_func;
	test->teardown = test_teardown;
	test->data = NULL;
	test->result.passed = true;
	test->result.msgs = NULL;
	test->result.start = 0;
	test->result.end = 0;

	group->tests = g_slist_append( group->tests, test );
}

void s_test_add_group( void* handle, const char* group_name, s_test_fp test_group ) {
	struct STest* stest = (struct STest*)handle;

	/*Create a new test group*/
	struct TestGroup* group = g_new( struct TestGroup, 1 );
	group->name = g_strndup( group_name, strlen( group_name ) );
	group->tests = NULL;

	stest->groups = g_slist_append( stest->groups, group );

	/*Call test group function to get all tests in group*/
	test_group( (void*)group );
}

int s_test_main( int argc, const char* argv[], void* handle )
{
	int retval = 0;
	struct STest* stest = (struct STest*)handle;

	// TODO Parse arguments

	// TODO Run all tests
	GSList* group_iter = NULL;
	GSList* test_iter = NULL;
	for( group_iter = stest->groups; group_iter; group_iter = group_iter->next ) {
		for( test_iter = ((struct TestGroup*)group_iter->data)->tests; test_iter;
				test_iter = test_iter->next ) {
			struct Test* test = (struct Test*)test_iter->data;
			struct timeval tv;

			gettimeofday( &tv, NULL );
			test->result.start = tv.tv_sec * 1000000 + tv.tv_usec;

			if( test->setup ) {
				test->setup( (void*)test );
			}
			
			test->test( (void*)test );

			if( test->teardown ) {
				test->teardown( (void*)test );
			}

			gettimeofday( &tv, NULL );
			test->result.end = tv.tv_sec * 1000000 + tv.tv_usec;
		}
	}

	// TODO Fix statistics
	for( group_iter = stest->groups; group_iter; group_iter = group_iter->next ) {
		for( test_iter = ((struct TestGroup*)group_iter->data)->tests; test_iter;
				test_iter = test_iter->next ) {
			struct Test* test = (struct Test*)test_iter->data;
			GSList* msg_iter = NULL;
			
			g_printf( "Test: %s - %s\n", test->name, test->result.passed ? "true" : "false" );
			g_printf( "Test start: %" PRIu64 ", end: %" PRIu64 ", length: %" PRIu64 "us\n",
					test->result.start, test->result.end, test->result.end - test->result.start );
			for( msg_iter = test->result.msgs; msg_iter; msg_iter = msg_iter->next ) {
				g_printf( "  -> %s\n", (gchar*)msg_iter->data );
			}
		}
	}

	// TODO Generate rapport

	// TODO Show result on stdout

	return retval;
}
