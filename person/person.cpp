#include "person.h"

Person::Person() : login_(""), password_("") {}

Person::Person(const std::string &login, const std::string &password)
    : login_(login), password_(password) {}

void Person::SetLogin(const std::string &login) { login_ = login; }

void Person::SetPassword(const std::string &password) { password_ = password; }

std::string Person::GetLogin() const { return login_; }

std::string Person::GetPassword() const { return password_; }

bool Person::operator<(const Person &user2) const {
  return login_[0] < user2.login_[0];
}

bool Person::operator==(const Person &user2) const {
  return login_ == user2.login_;
}