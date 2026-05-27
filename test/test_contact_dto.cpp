#include <criterion/criterion.h>
#include <string>

#include "dtos/contact_dto.hpp"

using namespace dws::dtos;

Test(contact_dto, valid_contact_passes_validation) {
    ContactDto dto{"John", "Doe", "john@example.com", "555-0123"};
    cr_expect(is_valid(dto), "A fully populated ContactDto should be valid");
}

Test(contact_dto, empty_first_name_fails_validation) {
    ContactDto dto{"", "Doe", "john@example.com", "555-0123"};
    cr_expect(!is_valid(dto), "Empty first name should fail validation");
}

Test(contact_dto, empty_last_name_fails_validation) {
    ContactDto dto{"John", "", "john@example.com", "555-0123"};
    cr_expect(!is_valid(dto), "Empty last name should fail validation");
}

Test(contact_dto, empty_email_fails_validation) {
    ContactDto dto{"John", "Doe", "", "555-0123"};
    cr_expect(!is_valid(dto), "Empty email should fail validation");
}

Test(contact_dto, empty_phone_fails_validation) {
    ContactDto dto{"John", "Doe", "john@example.com", ""};
    cr_expect(!is_valid(dto), "Empty phone should fail validation");
}

Test(contact_dto, first_name_exceeds_max_length_fails) {
    std::string long_name(101, 'A');
    ContactDto dto{long_name, "Doe", "john@example.com", "555-0123"};
    cr_expect(!is_valid(dto), "First name > 100 chars should fail validation");
}

Test(contact_dto, last_name_exceeds_max_length_fails) {
    std::string long_name(101, 'B');
    ContactDto dto{"John", long_name, "john@example.com", "555-0123"};
    cr_expect(!is_valid(dto), "Last name > 100 chars should fail validation");
}

Test(contact_dto, email_exceeds_max_length_fails) {
    std::string long_email(321, 'c');
    ContactDto dto{"John", "Doe", long_email, "555-0123"};
    cr_expect(!is_valid(dto), "Email > 320 chars should fail validation");
}

Test(contact_dto, phone_exceeds_max_length_fails) {
    std::string long_phone(31, '5');
    ContactDto dto{"John", "Doe", "john@example.com", long_phone};
    cr_expect(!is_valid(dto), "Phone > 30 chars should fail validation");
}

Test(contact_dto, email_without_at_sign_fails) {
    ContactDto dto{"John", "Doe", "no-at-sign.com", "555-0123"};
    cr_expect(!is_valid(dto), "Email without @ should fail validation");
}

Test(contact_dto, exactly_max_lengths_pass) {
    std::string name_100(100, 'N');
    std::string email_local(307, 'e');
    std::string email_full = email_local + "@x.co";  // 307 + 5 = 312 chars
    ContactDto dto{name_100, name_100, email_full, "555-0001"};
    cr_expect(is_valid(dto), "Exactly-at-limit fields should pass validation");
}

Test(contact_dto, to_contact_dto_extracts_correct_fields) {
    std::unordered_map<std::string, std::string,
                       drogon::utils::internal::SafeStringHash> params;
    params["firstname"] = "Alice";
    params["lastname"] = "Smith";
    params["email"] = "alice@example.com";
    params["phone"] = "555-9876";

    ContactDto dto = to_ContactDto(params);
    cr_expect_str_eq(dto.FirstName.c_str(), "Alice");
    cr_expect_str_eq(dto.LastName.c_str(), "Smith");
    cr_expect_str_eq(dto.EMail.c_str(), "alice@example.com");
    cr_expect_str_eq(dto.Phone.c_str(), "555-9876");
}

Test(contact_dto, to_contact_dto_handles_missing_fields) {
    std::unordered_map<std::string, std::string,
                       drogon::utils::internal::SafeStringHash> params;
    params["firstname"] = "Bob";
    // all other fields missing

    ContactDto dto = to_ContactDto(params);
    cr_expect_str_eq(dto.FirstName.c_str(), "Bob");
    cr_expect(dto.LastName.empty());
    cr_expect(dto.EMail.empty());
    cr_expect(dto.Phone.empty());
}
