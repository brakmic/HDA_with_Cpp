#pragma once
#include <soci/soci.h>

#include <set>
#include <string>
#include <vector>

#include "models/contact.hpp"
using namespace dws::models;

namespace dws {
namespace database {
class DbManager {
 public:
  DbManager();
  DbManager(const DbManager&) = delete;
  DbManager(DbManager&&) = delete;
  DbManager& operator=(const DbManager&) = delete;
  DbManager&& operator=(DbManager&&) = delete;
  virtual ~DbManager();
  Contact get_contact(unsigned long id);
  std::vector<Contact> get_contacts();
  // Parameterized search; safe against SQL injection.
  // column must be one of: firstname, lastname, email, phone.
  // Returns empty vector for unknown columns.
  std::vector<Contact> search_contacts(const std::string& column,
                                       const std::string& value);
  bool save_contact(const Contact& contact);
  bool update_contact(const Contact& contact);
  bool delete_contact(unsigned long id);
  // Import contacts from a CSV file. Expects a header line followed by
  // rows of FirstName,LastName,EMail,Phone. Returns the count imported.
  unsigned long import_from_csv(const std::string& filename);

 private:
  soci::session _db;
  static const std::set<std::string> ALLOWED_COLUMNS;
};
}  // namespace database
}  // namespace dws