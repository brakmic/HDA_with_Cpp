#pragma once
#include <soci/soci.h>

#include <string>
#include <vector>

#include "../models/contact.hpp"
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
  Contact get_contact(const std::string& where_args);
  std::vector<Contact> get_contacts();
  std::vector<Contact> get_contacts(const std::string& where_args);
  bool save_contact(const Contact& contact);
  bool update_contact(const Contact& contact);
  bool delete_contact(unsigned long id);

 private:
  soci::session _db;
};
}  // namespace database
}  // namespace dws