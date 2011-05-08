#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <sys/time.h>

#include "stest_internal.h"
#include "stdout_result.h"
#include "xml_result.h"
#include "update_statistics.h"

static struct Test* s_test_new_test( const char* name, s_test_fp test,
		s_test_fp setup, s_test_fp teardown );

static struct Test* s_test_new_test( const char* name, s_test_fp test_func,
		s_test_fp setup, s_test_fp teardown ) {
	struct Test* test = g_new( struct Test, 1 );
	test->name = g_strndup( name, strlen( name ) );
	test->setup = setup;
	test->test = test_func;
	test->teardown = teardown;
	test->data = NULL;
	test->result.passed = true;
	test->result.msgs = NULL;
	test->result.start = 0;
	test->result.end = 0;
	test->result.time = 0;

	return test;
}

static char* test_file_name(char* path)
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

void s_test_simple_test_result( void* handle, int passed, char* reason,
		const char* function, unsigned int line)
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

void s_test_assert_true( void* handle, int test, const char* function, unsigned int line)
{
	s_test_simple_test_result( handle, test, "Should of been true", function, line);
}

void s_test_assert_false( void* handle, int test, const char* function, unsigned int line)
{
	s_test_simple_test_result( handle, !test, "Should of been false", function, line);
}


void s_test_assert_int_equal( void* handle, int expected, int actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %d but was %d", expected, actual);
	s_test_simple_test_result( handle,expected==actual, s, function, line);
}

void s_test_assert_float_equal( void* handle, float expected, float actual, float delta, const char* function, unsigned int line )
{
	char s[100];
	float result = expected-actual;
	sprintf(s, "Expected %f but was %f", expected, actual);

	if (result < 0.0) {
		result = 0.0 - result;
	}

	s_test_simple_test_result( handle, result <= delta, s, function, line);
}

void s_test_assert_double_equal( void* handle, double expected, double actual, double delta, const char* function, unsigned int line )
{
	char s[100];
	double result = expected-actual;
	sprintf(s, "Expected %f but was %f", expected, actual);

	if (result < 0.0) {
		result = 0.0 - result;
	}

	s_test_simple_test_result( handle, result <= delta, s, function, line);
}

void s_test_assert_string_equal(void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s but was %s", expected, actual);
	s_test_simple_test_result(handle, strcmp(expected, actual)==0, s, function, line);
}

void s_test_assert_string_ends_with( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s to end with %s", actual, expected);
	s_test_simple_test_result( handle, strcmp(expected, actual+(strlen(actual)-strlen(expected)))==0, s, function, line);
}

void s_test_assert_string_starts_with( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s to start with %s", actual, expected);
	s_test_simple_test_result( handle, strncmp(expected, actual, strlen(expected))==0, s, function, line);
}

void s_test_assert_string_contains( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s to be in %s", expected, actual);
	s_test_simple_test_result( handle, strstr(actual, expected)!=NULL, s, function, line);
}

void s_test_assert_string_doesnt_contain( void* handle, char* expected, char* actual, const char* function, unsigned int line)
{
	char s[100];
	sprintf(s, "Expected %s not to have %s in it", actual, expected);
	s_test_simple_test_result(handle, strstr(actual, expected)==NULL, s, function, line);
}

/*
void s_test_header_printer(char* s, int length, char f)
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


void s_test_test_fixture_start(char* filepath)
{
	s_test_current_fixture = test_file_name(filepath);
	s_test_header_printer(s_test_current_fixture, 50, '-');
	s_test_fixture_tests_failed = sea_tests_failed;
	s_test_fixture_tests_run = sea_tests_run;
	s_test_fixture_teardown = 0;
	s_test_fixture_setup = 0;
}

void s_test_test_fixture_end()
{
	char s[100];
	sprintf(s, "%d run  %d failed", sea_tests_run-s_test_fixture_tests_run, sea_tests_failed-s_test_fixture_tests_failed);
	s_test_header_printer(s, 50, ' ');
	printf("\r\n");
}

static char* s_test_fixture_filter = 0;
static char* s_test_test_filter = 0;

void fixture_filter(char* filter)
{
	s_test_fixture_filter = filter;
}


void test_filter(char* filter)
{
	s_test_test_filter = filter;
}


int s_test_should_run( char* fixture, char* test)
{
	int run = 1;
	if (s_test_fixture_filter) {
		if (strncmp(s_test_fixture_filter, fixture, strlen(s_test_fixture_filter)) != 0) run = 0;
	}
	if (s_test_test_filter) {
		if (strncmp(s_test_test_filter, test, strlen(s_test_test_filter)) != 0) run = 0;
	}
	return run;
}
*/

static int run_tests( s_test_fp tests )
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

	stest->stat.tests = 0;
	stest->stat.failures = 0;
	stest->stat.test_groups = 0;
	stest->stat.time = 0;

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

	group->tests = g_slist_append(
			group->tests,
			s_test_new_test( test_name, test_func, NULL, NULL )
		);
}

void s_test_add_test_f( void* handle, const char* test_name,
		s_test_fp test_func, s_test_fp test_setup, s_test_fp test_teardown ) {
	struct TestGroup* group = (struct TestGroup*)handle;

	group->tests = g_slist_append(
			group->tests,
			s_test_new_test( test_name, test_func, test_setup, test_teardown )
		);
}

void s_test_add_group( void* handle, const char* group_name, s_test_fp test_group ) {
	struct STest* stest = (struct STest*)handle;

	/*Create a new test group*/
	struct TestGroup* group = g_new( struct TestGroup, 1 );

	group->name = g_strndup( group_name, strlen( group_name ) );
	group->tests = NULL;
	group->stat.tests = 0;
	group->stat.failures = 0;
	group->stat.time = 0;

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

	// Fix statistics
	s_test_update_statistics( stest );

	// Generate rapport
	s_test_write_xml_result( stest );

	// Show result on stdout
	s_test_write_stdout_result( stest );

	return retval;
}
