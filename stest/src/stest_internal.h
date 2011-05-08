#ifndef STEST_STEST_INTERNAL_H_INCLUDES
#define STEST_STEST_INTERNAL_H_INCLUDES

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include <glib.h>

#include "stest.h"

struct STestStat {
	uint64_t tests;
	uint64_t failures;
	uint64_t test_groups;
	uint64_t time;
};

struct STest {
	GSList* groups;
	struct STestStat stat;
};

struct TestGroupStat {
	uint64_t tests;
	uint64_t failures;
	uint64_t time;
};

struct TestGroup {
	gchar* name;
	GSList* tests;
	struct TestGroupStat stat;
};

struct TestResult {
	bool passed;
	GSList* msgs;

	uint64_t start;
	uint64_t end;
	uint64_t time;
};

struct Test {
	gchar* name;

	s_test_fp setup;
	s_test_fp test;
	s_test_fp teardown;

	void* data;

	struct TestResult result;
};

#endif
