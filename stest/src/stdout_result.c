#include "stdout_result.h"

void s_test_write_stdout_result( struct STest* stest ) {
	GSList* group_iter = NULL;
	GSList* test_iter = NULL;
	GSList* msg_iter = NULL;

	for( group_iter = stest->groups; group_iter; group_iter = group_iter->next ) {
		for( test_iter = ((struct TestGroup*)group_iter->data)->tests; test_iter;
				test_iter = test_iter->next ) {
			struct Test* test = (struct Test*)test_iter->data;

			g_printf( "Test: %s - %s\n", test->name, test->result.passed ? "true" : "false" );
			g_printf( "Test start: %" PRIu64 ", end: %" PRIu64 ", length: %" PRIu64 "us\n",
					test->result.start, test->result.end, test->result.end - test->result.start );
			for( msg_iter = test->result.msgs; msg_iter; msg_iter = msg_iter->next ) {
				g_printf( "  -> %s\n", (gchar*)msg_iter->data );
			}
		}
	}

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
	*/
}
