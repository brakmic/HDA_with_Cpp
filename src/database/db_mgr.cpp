#define FMT_HEADER_ONLY
#include "db_mgr.hpp"

#include <fmt/format.h>

#include <iostream>

using namespace dws::database;
using namespace dws::models;

DbManager::DbManager() : _db(session(sqlite3, "demo.db")) {}

DbManager::~DbManager() { _db.close(); }

Contact DbManager::get_contact(unsigned long id) {
  Contact c{};
  _db << fmt::format("select * from contacts where id = {}", id), into(c);
  return c;
}
/// @brief Returns a contact by using a where clause
/// @param where_args where clause elements, e.g. FirstName = 'John'
/// @return Contact object
Contact DbManager::get_contact(const std::string& where_args) {
  Contact c{};
  std::string query = "select * from contacts";
  _db.set_query_transformation([&where_args](std::string const& q) {
    return q + " where " + where_args;
  });
  _db << query, into(c);
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

std::vector<Contact> DbManager::get_contacts(const std::string& where_args) {
  Contact c{};
  std::vector<Contact> cs{};
  std::string query = "select * from contacts";
  _db.set_query_transformation([&where_args](std::string const& q) {
    return q + " where " + where_args;
  });
  statement st = (_db.prepare << query, into(c));
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
