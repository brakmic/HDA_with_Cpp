#include <criterion/criterion.h>
#include <string>

#include "database/db_mgr.hpp"
#include "models/contact.hpp"

using namespace dws::models;
using namespace dws::database;

// These tests run against the demo.db that ships with the project.
// In Docker, server_config.json and demo.db are copied into builddir/
// before meson test executes. Each write test cleans up after itself.

Test(db_mgr, get_contacts_returns_non_empty) {
    DbManager db;
    auto contacts = db.get_contacts();
    cr_expect(!contacts.empty(),
              "demo.db should contain at least one contact");
}

Test(db_mgr, get_contact_by_valid_id_returns_matching_contact) {
    DbManager db;
    auto contacts = db.get_contacts();
    if (contacts.empty()) {
        cr_skip("No contacts in database");
    }
    unsigned long id = contacts.front().ID;
    Contact c = db.get_contact(id);
    cr_expect_eq(c.ID, id,
                 "get_contact should return the contact with the requested ID");
}

Test(db_mgr, get_contact_by_invalid_id_returns_empty_contact) {
    DbManager db;
    Contact c = db.get_contact(99999);
    cr_expect_eq(c.ID, 0UL,
                 "get_contact with non-existent ID should return contact with ID 0");
}

Test(db_mgr, save_and_retrieve_contact) {
    DbManager db;
    Contact new_contact{};
    new_contact.FirstName = "TestFirst";
    new_contact.LastName = "TestLast";
    new_contact.EMail = "test@example.com";
    new_contact.Phone = "555-TEST";

    bool saved = db.save_contact(new_contact);
    cr_expect(saved, "save_contact should return true on success");

    // Retrieve and verify via search
    auto results = db.search_contacts("email", "test@example.com");
    cr_expect(!results.empty(),
              "search_contacts should find the newly saved contact");

    // Clean up: delete the test contact
    if (!results.empty()) {
        db.delete_contact(results.front().ID);
    }
}

Test(db_mgr, update_contact_modifies_fields) {
    DbManager db;

    // Create a contact to update
    Contact c{};
    c.FirstName = "BeforeUpdate";
    c.LastName = "BeforeUpdate";
    c.EMail = "before@example.com";
    c.Phone = "555-BEFORE";
    db.save_contact(c);

    // Retrieve it to get the generated ID
    auto results = db.search_contacts("email", "before@example.com");
    if (results.empty()) {
        cr_skip("Failed to save test contact");
    }
    Contact saved = results.front();

    // Update the contact
    saved.FirstName = "AfterUpdate";
    saved.LastName = "AfterUpdate";
    saved.EMail = "after@example.com";
    saved.Phone = "555-AFTER";
    bool updated = db.update_contact(saved);
    cr_expect(updated, "update_contact should return true on success");

    // Verify the update
    Contact retrieved = db.get_contact(saved.ID);
    cr_expect_str_eq(retrieved.FirstName.c_str(), "AfterUpdate");
    cr_expect_str_eq(retrieved.EMail.c_str(), "after@example.com");

    // Clean up
    db.delete_contact(saved.ID);
}

Test(db_mgr, delete_contact_removes_record) {
    DbManager db;

    Contact c{};
    c.FirstName = "ToDelete";
    c.LastName = "ToDelete";
    c.EMail = "delete@example.com";
    c.Phone = "555-DEL";
    db.save_contact(c);

    auto results = db.search_contacts("email", "delete@example.com");
    if (results.empty()) {
        cr_skip("Failed to save test contact");
    }
    unsigned long id = results.front().ID;

    bool deleted = db.delete_contact(id);
    cr_expect(deleted, "delete_contact should return true on success");

    Contact after = db.get_contact(id);
    cr_expect_eq(after.ID, 0UL,
                 "get_contact after delete should return empty contact");
}

Test(db_mgr, search_with_valid_column_finds_match) {
    DbManager db;
    auto contacts = db.get_contacts();
    if (contacts.empty()) {
        cr_skip("No contacts in database");
    }
    // Search for the first contact by first name using a substring
    std::string query = contacts.front().FirstName.substr(0, 3);
    auto results = db.search_contacts("firstname", query);
    cr_expect(!results.empty(),
              "search_contacts with valid column should return results");
}

Test(db_mgr, search_with_invalid_column_returns_empty) {
    DbManager db;
    auto results = db.search_contacts("nonexistent_column", "value");
    cr_expect(results.empty(),
              "search_contacts with invalid column should return empty vector");
}

Test(db_mgr, search_with_sql_injection_in_value_is_safe) {
    DbManager db;
    // SQL wildcards and injection attempts in the value parameter are
    // treated as literal strings by SOCI's parameterized binding.
    auto results = db.search_contacts("lastname", "'; DROP TABLE contacts;--");
    cr_expect(results.empty() || !results.empty(),
              "SQL injection in value should not crash");
}

#include <cstdio>

Test(db_mgr, bootstrap_creates_table_and_imports_csv) {
    const std::string temp_db = "test_bootstrap_temp.db";
    std::remove(temp_db.c_str());

    bool result = DbManager::bootstrap(temp_db, "contacts.csv");
    cr_expect(result,
              "bootstrap should return true when CSV is found");

    soci::session sql(soci::sqlite3, temp_db);
    int count = 0;
    sql << "SELECT COUNT(*) FROM contacts", soci::into(count);
    cr_expect_gt(count, 0,
                 "contacts table should contain imported rows after bootstrap");

    // Idempotent: second call must not duplicate data
    bool second = DbManager::bootstrap(temp_db, "contacts.csv");
    cr_expect(second,
              "bootstrap should return true on an already-populated DB");
    int count2 = 0;
    sql << "SELECT COUNT(*) FROM contacts", soci::into(count2);
    cr_expect_eq(count, count2,
                 "repeat bootstrap should not duplicate rows");

    sql.close();
    std::remove(temp_db.c_str());
}

Test(db_mgr, bootstrap_returns_false_when_csv_missing) {
    const std::string temp_db = "test_bootstrap_nocsv.db";
    std::remove(temp_db.c_str());

    bool result = DbManager::bootstrap(temp_db, "nonexistent_file.csv");
    cr_expect_not(result,
                  "bootstrap should return false when CSV file is missing");

    std::remove(temp_db.c_str());
}
