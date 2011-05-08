#include <glib/gprintf.h>

#include "stdout_result.h"

void s_test_write_stdout_result( struct STest* stest ) {

	g_printf("\r\n\r\n==================================================\r\n");
	if ( stest->stat.failures > 0) {
		g_printf("                      Failed\r\n");
	} else {
		g_printf("               ALL TESTS PASSED\r\n");
	}
	g_printf("                 %" PRIu64 " tests run\r\n", stest->stat.tests);
	g_printf("                    in %" PRIu64 " us\r\n", stest->stat.time );
	g_printf("==================================================\r\n");
}
