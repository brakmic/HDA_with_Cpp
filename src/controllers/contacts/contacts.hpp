#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class Contacts : public drogon::HttpController<Contacts> {
 public:
  METHOD_LIST_BEGIN
  METHOD_ADD(Contacts::list, "", Get, Post);
  METHOD_ADD(Contacts::list, "/", Get, Post);
  METHOD_ADD(Contacts::get_by_id, "/{id}", Get);
  METHOD_ADD(Contacts::create_new, "/new", Get);
  METHOD_ADD(Contacts::create, "/new", Post);
  METHOD_ADD(Contacts::edit, "/{id}/edit", Get);
  METHOD_ADD(Contacts::update, "/{id}/edit", Put);
  METHOD_ADD(Contacts::delete_, "/{id}/delete", Delete);

  METHOD_LIST_END
  void list(const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback);
  void get_by_id(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback,
                unsigned long id);
  void create_new(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback);
  void create(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback);
  void edit(const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback,
            unsigned long id);
  void update(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback,
              unsigned long id);
  void delete_(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback,
              unsigned long id);
};
