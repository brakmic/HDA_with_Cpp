#include "home.hpp"

// add definition of your processing function here
void Home::get(const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback) {
  Json::Value ret;
  ret["result"] = "OK";
  ret["server"] = "drogon";
  auto res = HttpResponse::newHttpJsonResponse(ret);
  res->setStatusCode(HttpStatusCode::k200OK);
  callback(res);
}
