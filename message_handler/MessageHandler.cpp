#include "MessageHandler.h"
#include <chrono>
#include <memory>

MessageHandler::MessageHandler(const std::string &message, Database &database) {
  size_t messageJsonLength = message.length();
  JSONCPP_STRING err;
  Json::CharReaderBuilder builder;
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

  if (!reader->parse(message.c_str(), message.c_str() + messageJsonLength,
                     &root_, &err)) {
    response_ = RequestNotDefined();
    return;
  }

  const std::string Request_Type = root_["Request"].asString();

  if (Request_Type == "Registration") {
    response_ = RequestRegistration(database);
  } else if (Request_Type == "Authorize") {
    response_ = RequestAuthorization(database);
  } else if (Request_Type == "Change password") {
    response_ = RequestChangePassword(database);
  } else if (Request_Type == "Delete account") {
    response_ = RequestDeleteAccout(database);
  } else if (Request_Type == "Clean database") {
    response_ = RequestCleanDatabase(database);
  } else if (Request_Type == "Message") {
    response_ = RequestMessage(database);
  } else {
    response_ = RequestNotDefined();
  }
}

std::string MessageHandler::GetResponse() const { return response_; }

std::string MessageHandler::RequestNotDefined() const {
  Json::Value json;
  SetRequestStatus(json, "Request_Ans", "Fail", "Request not defined");

  return JsonToString(json);
}

std::string MessageHandler::RequestRegistration(Database &database) const {
  Person user(root_["Login"].asString(), root_["Password"].asString());

  if (!database.FindUser(user)) {
    database.AddData(user);

    Json::Value json;
    SetRequestStatus(json, "Registration_Ans", "OK", "");

    return JsonToString(json);
  } else {
    Json::Value json;
    SetRequestStatus(json, "Registration_Ans", "Fail",
                     "Account already exists");

    return JsonToString(json);
  }
}

std::string MessageHandler::RequestAuthorization(Database &database) const {
  Person user(root_["Login"].asString(), root_["Password"].asString());

  if (database.FindUser(user)) {
    Json::Value json;
    SetRequestStatus(json, "Authorization_Ans", "OK", "");

    return JsonToString(json);
  } else {
    Json::Value json;
    SetRequestStatus(json, "Authorization_Ans", "Fail", "Account not found");

    return JsonToString(json);
  }
}

std::string MessageHandler::RequestChangePassword(Database &database) const {
  Person user(root_["Login"].asString(), root_["Old_password"].asString());

  if (database.FindUser(user)) {
    if (root_["New_password"].asString() != user.GetPassword()) {
      database.ChangePassword(user, root_["New_password"].asString());

      Json::Value json;
      SetRequestStatus(json, "Change password_Ans", "OK", "");

      return JsonToString(json);
    } else {
      Json::Value json;
      SetRequestStatus(json, "Change password_Ans", "Fail", "Equal passwords");

      return JsonToString(json);
    }
  } else {
    Json::Value json;
    SetRequestStatus(json, "Change password_Ans", "Fail",
                     "Account doesn't exist");

    return JsonToString(json);
  }
}

std::string MessageHandler::RequestDeleteAccout(Database &database) const {
  Person user(root_["Login"].asString(), root_["Password"].asString());

  if (database.FindUser(user)) {
    database.DeleteAccount(user);

    Json::Value json;
    SetRequestStatus(json, "Delete account_Ans", "OK", "");

    return JsonToString(json);
  } else {
    Json::Value json;
    SetRequestStatus(json, "Delete account_Ans", "Fail", "Account not found");

    return JsonToString(json);
  }
}

std::string MessageHandler::RequestCleanDatabase(Database &database) const {
  database.CleanDatabase();

  Json::Value json;
  SetRequestStatus(json, "Clean database_Ans", "OK", "");

  return JsonToString(json);
}

std::string MessageHandler::RequestMessage(Database &database) const {
  auto now = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(now);
  std::string time = std::ctime(&end_time);

  Json::Value json;
  SetRequestStatus(json, "Message_Ans", "OK", "");
  json["Time"] = time;

  return JsonToString(json);
}

std::string MessageHandler::JsonToString(Json::Value &json) const {
  Json::FastWriter fastWriter;
  std::string result = fastWriter.write(json);

  return result;
}

void MessageHandler::SetRequestStatus(Json::Value &json,
                                      const std::string &request,
                                      const std::string &code,
                                      const std::string &status) const {
  json["Request"] = request;
  json["Code"] = code;
  json["Status"] = status;
}