#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

#include <string>
#include <vector>
using namespace soci;

namespace dws {
namespace models {
struct Contact {
  unsigned long ID;
  std::string FirstName;
  std::string LastName;
  std::string EMail;
  std::string Phone;
};

}  // namespace models
}  // namespace dws

/// @brief Type Conversions
/// Check https://soci.sourceforge.net/doc/release/4.0/types/
namespace soci {
template <>
struct type_conversion<dws::models::Contact> {
  typedef values base_type;
  static void from_base(const values& v, indicator /*ind*/,
                        dws::models::Contact& c) {
    c.ID = v.get<int>("ID");
    c.FirstName = v.get<std::string>("FirstName");
    c.LastName = v.get<std::string>("LastName");
    c.EMail = v.get<std::string>("EMail");
    c.Phone = v.get<std::string>("Phone");
  }

  static void to_base(const dws::models::Contact& c, values& v,
                      indicator& ind) {
    v.set("ID", c.ID);
    v.set("FirstName", c.FirstName);
    v.set("LastName", c.LastName);
    v.set("EMail", c.EMail);
    v.set("Phone", c.Phone);
    ind = i_ok;
  }
};
}  // namespace soci
