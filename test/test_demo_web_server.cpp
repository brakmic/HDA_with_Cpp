#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <string.h>

#include "../src/server/server_config.hpp"

ReportHook(PRE_INIT)(struct criterion_test *test) {
  printf("testing %s in category %s\n", test->name, test->category);
}

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
  printf("Asserts: [%d passed, %d failed, %d total]\n", stats->passed_asserts,
         stats->failed_asserts, stats->passed_asserts + stats->failed_asserts);
}

ReportHook(PRE_ALL)(struct criterion_test_set *tests) {
  (void)tests;
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  puts("init");
}

ReportHook(POST_ALL)(struct criterion_global_stats *stats) {
  (void)stats;
  puts("end");
}

void setup(void) {}

void teardown(void) {}

Test(server_config, passing) {
  ServerConfig config;
  cr_expect(config.to_json().isNull() == false,
            "ServerConfig should have read JSON");
  cr_expect(config.get("database", "file").empty() == false,
            "ServerConfig should return SQLite file from JSON");
}
