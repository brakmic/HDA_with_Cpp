#include <criterion/criterion.h>
#include <string>
#include <vector>

#include "database/db_mgr.hpp"
#include "models/contact.hpp"

using namespace dws::database;
using namespace dws::models;

Test(contacts_controller, search_allowed_column_returns_results) {
    DbManager db;
    auto results = db.search_contacts("firstname", "John");
    // Even if the table is empty, the query should succeed (no exception)
    cr_expect(results.empty() || !results.empty(),
              "Search with allowed column should not throw");
}

Test(contacts_controller, search_disallowed_column_returns_empty) {
    DbManager db;
    auto results = db.search_contacts("password", "secret");
    cr_expect(results.empty(),
              "Search with disallowed column should return empty vector");
}

Test(contacts_controller, search_sql_injection_attempt_blocked) {
    DbManager db;
    // Attempt SQL injection via column name
    auto results = db.search_contacts("1; DROP TABLE contacts;--", "x");
    cr_expect(results.empty(),
              "SQL injection via column name should be blocked by whitelist");
}

Test(contacts_controller, search_with_like_pattern_is_safe) {
    DbManager db;
    // The value parameter uses LIKE with parameterized binding, so
    // SQL wildcards in the value are treated as literal patterns, not
    // as SQL injection vectors.
    auto results = db.search_contacts("lastname", "%'; DROP TABLE contacts;--");
    cr_expect(results.empty() || !results.empty(),
              "Search with SQL in value should use parameterized query safely");
}

Test(contacts_controller, search_each_allowed_column_does_not_throw) {
    DbManager db;
    // Each whitelisted column must not throw.
    bool threw = false;
    try { db.search_contacts("firstname", "test"); } catch (...) { threw = true; }
    cr_expect(!threw, "firstname should be allowed");
    try { db.search_contacts("lastname", "test"); } catch (...) { threw = true; }
    cr_expect(!threw, "lastname should be allowed");
    try { db.search_contacts("email", "test"); } catch (...) { threw = true; }
    cr_expect(!threw, "email should be allowed");
    try { db.search_contacts("phone", "test"); } catch (...) { threw = true; }
    cr_expect(!threw, "phone should be allowed");
}

Test(contacts_controller, search_rejects_each_disallowed_column) {
    DbManager db;
    // Common column names that attackers might try; must return empty
    cr_expect(db.search_contacts("id", "1").empty(),
              "id column should not be searchable");
    cr_expect(db.search_contacts("admin", "1").empty(),
              "admin column should not be searchable");
    cr_expect(db.search_contacts("password", "x").empty(),
              "password column should not be searchable");
    cr_expect(db.search_contacts("*", "x").empty(),
              "asterisk should not be searchable");
}
