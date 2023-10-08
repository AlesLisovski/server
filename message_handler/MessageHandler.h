#pragma once
#include "database/database.h"
#include "person/person.h"
#include "json/json-forwards.h"
#include "json/json.h"
#include <string>

class MessageHandler {
  Json::Value root_;
  std::string response_;

public:
  MessageHandler(const std::string &message, Database &database);

  std::string GetResponse() const;
  std::string RequestNotDefined() const;
  std::string RequestRegistration(Database &database) const;
  std::string RequestAuthorization(Database &database) const;
  std::string RequestChangePassword(Database &database) const;
  std::string RequestDeleteAccout(Database &database) const;
  std::string RequestCleanDatabase(Database &database) const;
  std::string RequestMessage(Database &database) const;
  std::string JsonToString(Json::Value &json) const;

  void SetRequestStatus(Json::Value &json, const std::string &request,
                        const std::string &code,
                        const std::string &status) const;
};
