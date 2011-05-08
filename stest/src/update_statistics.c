#include "update_statistics.h"

void s_test_update_statistics( struct STest* stest ) {
	GSList* group_iter = NULL;
	GSList* test_iter = NULL;
	struct TestGroup* c_group = NULL;
	struct Test* c_test = NULL;

	for( group_iter = stest->groups; group_iter; group_iter = group_iter->next ) {
		c_group = (struct TestGroup*)group_iter->data;
		stest->stat.test_groups += 1;

		for( test_iter = c_group->tests; test_iter; test_iter = test_iter->next ) {
			c_test = (struct Test*)test_iter->data;

			c_test->result.time = c_test->result.end - c_test->result.start;

			stest->stat.tests += 1;
			stest->stat.failures += c_test->result.passed ? 0 : 1;
			stest->stat.time += c_test->result.time;

			c_group->stat.tests += 1;
			c_group->stat.failures += c_test->result.passed ? 0 : 1;
			c_group->stat.time += c_test->result.time;
		}
	}
}
