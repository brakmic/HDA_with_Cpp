
#define FMT_HEADER_ONLY
#include "contacts.hpp"

#include <fmt/format.h>

#include <unordered_map>
#include <utility>

#include "../database/db_mgr.hpp"
#include "../dtos/contact_dto.hpp"

using namespace dws::database;
using namespace dws::dtos;

void Contacts::list(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback) {
  DbManager db;
  HttpViewData data;

  auto params = req->getParameters();
  if (params.size() > 0) {
    auto p = *params.begin();
    std::string column = p.first;
    std::string value = p.second;
    auto contact =
        db.get_contacts(fmt::format("{} like '%{}%'", column, value));
    data.insert("contacts", contact);
  } else {
    data.insert("contacts", db.get_contacts());
  }
  auto res = HttpResponse::newHttpViewResponse("contacts", data);
  callback(res);
}

void Contacts::get_by_id(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback, unsigned long id) {
  DbManager db;
  HttpViewData data;
  data.insert("contact", db.get_contact(id));
  auto res = HttpResponse::newHttpViewResponse("contact", data);
  callback(res);
}

void Contacts::create_new(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  HttpViewData data;
  ContactDto dto = to_ContactDto(req->getParameters());
  data.insert("contact", dto);
  auto res = HttpResponse::newHttpViewResponse("contact_new", data);
  callback(res);
}

void Contacts::create(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback) {
  DbManager db;
  HttpResponsePtr res;
  auto dto = to_ContactDto(req->getParameters());
  if (is_valid(dto)) {
    Contact c{
        .FirstName = dto.FirstName,
        .LastName = dto.LastName,
        .EMail = dto.EMail,
        .Phone = dto.Phone,
    };
    bool success = db.save_contact(c);
    // return all contacts via GET /contacts request
    list(HttpRequest::newHttpRequest(),
         std::forward<decltype(callback)>(callback));
  } else {
    HttpViewData data;
    data.insert("contact", dto);
    res = HttpResponse::newHttpViewResponse("contact_new", data);
    callback(res);
  }
}

void Contacts::edit(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback,
                    unsigned long id) {
  DbManager db;
  HttpViewData data;
  data.insert("contact", db.get_contact(id));
  auto res = HttpResponse::newHttpViewResponse("contact_edit", data);
  callback(res);
}

void Contacts::update(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback,
                      unsigned long id) {
  DbManager db;
  HttpResponsePtr res = HttpResponse::newHttpResponse();
  auto existing = db.get_contact(id);
  auto dto = to_ContactDto(req->getParameters());
  if (existing.ID == id && is_valid(dto)) {
    existing.FirstName = dto.FirstName;
    existing.LastName = dto.LastName;
    existing.EMail = dto.EMail;
    existing.Phone = dto.Phone;
    db.update_contact(existing);
  }
  // return all contacts via GET / contacts request
  list(HttpRequest::newHttpRequest(),
       std::forward<decltype(callback)>(callback));
}

void Contacts::delete_(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       unsigned long id) {
  DbManager db;
  HttpResponsePtr res = HttpResponse::newHttpResponse();
  auto existing = db.get_contact(id);
  if (existing.ID) {
    db.delete_contact(id);
  }
  callback(res);
}
