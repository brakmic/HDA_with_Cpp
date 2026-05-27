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
  static const size_t MAX_NAME_LENGTH = 100;
  static const size_t MAX_EMAIL_LENGTH = 320;
  static const size_t MAX_PHONE_LENGTH = 30;

  if (dto.FirstName.empty() || dto.FirstName.length() > MAX_NAME_LENGTH) {
    return false;
  }
  if (dto.LastName.empty() || dto.LastName.length() > MAX_NAME_LENGTH) {
    return false;
  }
  if (dto.EMail.empty() || dto.EMail.length() > MAX_EMAIL_LENGTH) {
    return false;
  }
  if (dto.EMail.find('@') == std::string::npos) {
    return false;
  }
  if (dto.Phone.empty() || dto.Phone.length() > MAX_PHONE_LENGTH) {
    return false;
  }
  return true;
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