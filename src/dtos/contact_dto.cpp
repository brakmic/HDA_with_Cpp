#include "contact_dto.hpp"

#include <string>
#include <unordered_map>

namespace dws {
namespace dtos {
ContactDto to_ContactDto(
    const std::unordered_map<std::string, std::string,
                             drogon::utils::internal::SafeStringHash>& map) {
  ContactDto dto{.FirstName = get_value(map, "firstname"),
                 .LastName = get_value(map, "lastname"),
                 .EMail = get_value(map, "email"),
                 .Phone = get_value(map, "phone")};
  return dto;
}

bool is_valid(const ContactDto& dto) {
  return !dto.FirstName.empty() && !dto.LastName.empty() &&
         !dto.EMail.empty() && !dto.Phone.empty();
}

std::string get_value(
    const std::unordered_map<std::string, std::string,
                             drogon::utils::internal::SafeStringHash>& map,
    std::string key) {
  if (map.find(key) != map.end()) {
    return map.find(key)->second;
  }
  return std::string{};
}

}  // namespace dtos
}  // namespace dws