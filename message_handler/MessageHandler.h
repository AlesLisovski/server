#pragma once
#include "database/database.h"
#include "person/person.h"
#include "json/json-forwards.h"
#include "json/json.h"
#include <string>

class MessageHandler {
  Json::Value root_;
  std::string response_;
  int socket_status_ = SOCKET_STATUS::PASSIVE_SOCKET;
  std::string login_ = "";

public:
  MessageHandler(const std::string &message, Database &database);

  enum SOCKET_STATUS {
    CREATE_SOCKET,
    DELETE_SOCKET,
    PASSIVE_SOCKET,
  };

  std::string GetResponse() const;
  std::string GetLogin() const;
  std::string RequestNotDefined() const;
  std::string RequestRegistration(Database &database);
  std::string RequestAuthorization(Database &database);
  std::string RequestChangePassword(Database &database) const;
  std::string RequestDeleteAccout(Database &database) const;
  std::string RequestCleanDatabase(Database &database) const;
  std::string RequestMessage(Database &database) const;
  std::string JsonToString(const Json::Value &json) const;

  void RequestAppClosed();
  void SetLogin(const std::string &login);
  void SetRequestStatus(Json::Value &json, const std::string &request,
                        const std::string &code,
                        const std::string &status) const;

  int GetSocketStatus() const;
};
