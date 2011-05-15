#include <glib/gstdio.h>
#include <glib/gprintf.h>

#include "xml_result.h"

/* XML report format
 *
 * <testsuites>     1 the aggregated result of all junit testfiles
 *   <testsuite>    * the output from a single TestSuite
 *     name         Name of test suite
 *     tests        The total number of tests
 *     errors       The total number of errors
 *     failures     The total number of failures
 *     time         The total execution time
 *     timestamp    The date/time when testsuite was executed ( in ISO8601 format )
 *
 *                  Success rate = (tests - errors - failures) / tests
 *     <properties> 1 the defined properties at test execution
 *       <property> * name/value pair for a single property
 *     <testcase>   * the results from executing a test method
 *       name       Name of test case
 *       classname  Full classname of test (needed?)
 *       time       The total execution time
 *
 *       <failure>  * Failure text
 *         message  Human readable failure text
 *         type     Failure type
 *       <error>    * Error text
 *         message  Human readable error text
 *         type     Error type
 *   <system-out> 1 data written to System.out during the test run
 *   <system-err> 1 data written to System.err during the test run
 *
 */

const char* XMLDEC = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
const char* START_TESTSUITES = "<testsuites>\n";
const char* END_TESTSUITES = "</testsuites>\n";
const char* START_TESTSUITE = "<testsuite name=\"%s\" tests=\"%" PRIu64 "\" errors=\"%" PRIu64 "\" failures=\"%" PRIu64 "\" time=\"%" PRIu64 "\" timestamp=\"%s\">\n";
const char* END_TESTSUITE = "</testsuite>\n";
const char* START_TEST = "<testcase name=\"%s\" classname=\"%s.%s\" time=\"%" PRIu64 "\">\n";
const char* END_TEST = "</testcase>\n";
const char* START_FAILURE = "<failure message=\"%s\" type=\"%s\">\n";
const char* END_FAILURE = "</failure>\n";
const char* START_ERROR = "<error message=\"%s\" type=\"%s\">\n";
const char* END_ERROR = "</error>\n";
const char* test_result_fname = "TESTSUITE.xml";

void s_test_write_xml_result( struct STest* stest ) {
	FILE* fdis = g_fopen( test_result_fname, "w" );
	if( fdis == NULL ) {
		// TODO Error handling
		return;
	}

	// Print beginning of report
	g_fprintf( fdis, "%s%s", XMLDEC, START_TESTSUITES );

	for( GSList* group_iter = stest->groups; group_iter; group_iter = group_iter->next ) {
		struct TestGroup* c_group = (struct TestGroup*)group_iter->data;
		
		// Print test suite header
		g_fprintf( fdis, START_TESTSUITE, c_group->name, c_group->stat.tests, 0,
				c_group->stat.failures, c_group->stat.time, "" );

		for( GSList* test_iter = c_group->tests; test_iter; test_iter = test_iter->next ) {
			struct Test* c_test = (struct Test*)test_iter->data;

			// print test case header
			g_fprintf( fdis, START_TEST, c_test->name, c_group->name, c_test->name, c_test->result.time );

			// Show failure
			if( ! c_test->result.passed ) {
				g_fprintf( fdis, START_FAILURE, "Assert failed", "" );

				for( GSList* msg_iter = c_test->result.msgs; msg_iter; msg_iter = msg_iter->next ) {
					g_fprintf( fdis, "%s\n", (gchar*)msg_iter->data );
				}

				g_fprintf( fdis, "%s", END_FAILURE );
			}

			g_fprintf( fdis, "%s", END_TEST );
		}

		g_fprintf( fdis, "%s", END_TESTSUITE );
	}
	g_fprintf( fdis, "%s", END_TESTSUITES );

	fclose( fdis );
}
