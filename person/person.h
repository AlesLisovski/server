#pragma once
#include <fstream>
#include <string>

struct Person {
private:
  std::string login_;
  std::string password_;

public:
  Person();
  Person(const std::string &login, const std::string &password);
  void SetLogin(const std::string &login);
  void SetPassword(const std::string &password);
  std::string GetLogin() const;
  std::string GetPassword() const;

  bool operator<(const Person &user2) const;
  bool operator==(const Person &user2) const;

  friend std::istream &operator>>(std::istream &in, Person &person) {
    std::string login, password;
    in >> login >> password;

    person.SetLogin(login);
    person.SetPassword(password);

    return in;
  }

  friend std::ostream &operator<<(std::ostream &os, const Person &person) {
    return os << person.GetLogin() << " " << person.GetPassword();
  }
};