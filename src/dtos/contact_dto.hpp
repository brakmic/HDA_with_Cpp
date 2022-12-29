#pragma once
#include <drogon/utils/Utilities.h>

#include <string>
#include <unordered_map>

namespace dws {
namespace dtos {
struct ContactDto {
  std::string FirstName;
  std::string LastName;
  std::string EMail;
  std::string Phone;
};

ContactDto to_ContactDto(
    const std::unordered_map<std::string, std::string,
                             drogon::utils::internal::SafeStringHash>& map);

bool is_valid(const ContactDto& dto);

std::string get_value(
    const std::unordered_map<std::string, std::string,
                             drogon::utils::internal::SafeStringHash>& map,
    std::string key);

}  // namespace dtos
}  // namespace dws