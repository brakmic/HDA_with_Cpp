#include "home.hpp"

void Home::get(const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback) {
  auto res = HttpResponse::newRedirectionResponse("/contacts");
  callback(res);
}
