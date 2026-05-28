#define FMT_HEADER_ONLY
#include "db_mgr.hpp"

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "server/server_config.hpp"

using namespace dws::database;
using namespace dws::models;

// Whitelist of column names allowed in search queries.
// Any column not in this set is rejected to prevent SQL injection
// via the column name parameter.
const std::set<std::string> DbManager::ALLOWED_COLUMNS = {
    "firstname", "lastname", "email", "phone"
};

DbManager::DbManager()
    : _db(session(sqlite3, ServerConfig::instance().get("database", "file"))) {}

DbManager::~DbManager() { _db.close(); }

Contact DbManager::get_contact(unsigned long id) {
  Contact c{};
  _db << "select * from contacts where id = :id", use(id), into(c);
  return c;
}

std::vector<Contact> DbManager::get_contacts() {
  Contact c{};
  std::vector<Contact> cs{};
  statement st = (_db.prepare << "select * from contacts", into(c));
  st.execute();
  while (st.fetch()) {
    cs.push_back(c);
  }
  return cs;
}

std::vector<Contact> DbManager::search_contacts(
    const std::string& column, const std::string& value) {
  // Reject unknown column names to prevent SQL injection.
  if (ALLOWED_COLUMNS.find(column) == ALLOWED_COLUMNS.end()) {
    return {};
  }

  Contact c{};
  std::vector<Contact> cs{};
  std::string query =
      "select * from contacts where " + column + " like :value";
  std::string pattern = "%" + value + "%";
  statement st = (_db.prepare << query,
                  use(pattern),
                  into(c));
  st.execute();
  while (st.fetch()) {
    cs.push_back(c);
  }
  return cs;
}

bool DbManager::save_contact(const Contact& contact) {
  statement st(
      (_db.prepare << "insert into contacts(firstname, lastname, email, phone) "
                      "values(:fn, :ln, :em, :ph)",
       use(contact.FirstName), use(contact.LastName), use(contact.EMail),
       use(contact.Phone)));
  st.execute(true);
  return (st.get_affected_rows() > 0);
}

bool DbManager::update_contact(const Contact& contact) {
  statement st(
      (_db.prepare
           << "update contacts"
              " set firstname = :fn, lastname = :ln, email = :em, phone = :pn"
              " where id = :id",
       use(contact.FirstName), use(contact.LastName), use(contact.EMail),
       use(contact.Phone), use(contact.ID)));
  st.execute(true);
  return (st.get_affected_rows() > 0);
}

bool DbManager::delete_contact(unsigned long id) {
  statement st((_db.prepare << "delete from contacts"
                               " where id = :id",
                use(id)));
  st.execute(true);
  return (st.get_affected_rows() > 0);
}

unsigned long DbManager::import_from_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return 0;
    }
    std::string line;
    std::getline(file, line);  // discard header
    unsigned long count = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string firstName, lastName, email, phone;
        if (std::getline(ss, firstName, ',') &&
            std::getline(ss, lastName, ',') &&
            std::getline(ss, email, ',') &&
            std::getline(ss, phone, ',')) {
            Contact c{};
            c.FirstName = firstName;
            c.LastName = lastName;
            c.EMail = email;
            c.Phone = phone;
            if (save_contact(c)) {
                ++count;
            }
        }
    }
    return count;
}

bool DbManager::bootstrap(const std::string& db_file,
                          const std::string& csv_file) {
    soci::session sql(soci::sqlite3, db_file);

    sql << "CREATE TABLE IF NOT EXISTS contacts ("
           "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
           "firstname TEXT, "
           "lastname TEXT, "
           "email TEXT, "
           "phone TEXT"
           ")";

    int count = 0;
    sql << "SELECT COUNT(*) FROM contacts", soci::into(count);

    if (count > 0) {
        return true;
    }

    std::ifstream file(csv_file);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::getline(file, line);  // discard header

    unsigned long imported = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fn, ln, em, ph;
        if (std::getline(ss, fn, ',') &&
            std::getline(ss, ln, ',') &&
            std::getline(ss, em, ',') &&
            std::getline(ss, ph, ',')) {
            sql << "INSERT INTO contacts(firstname, lastname, email, phone) "
                   "VALUES(:fn, :ln, :em, :ph)",
                soci::use(fn), soci::use(ln), soci::use(em), soci::use(ph);
            ++imported;
        }
    }

    return imported > 0;
}
