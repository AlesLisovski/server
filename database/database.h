#pragma once
#include "person/person.h"
#include <algorithm>
#include <vector>

class Database {
  std::vector<Person> data_;
  std::string database_;

public:
  Database(const std::string &file_name);

  int Size() const;
  bool Empty() const;
  bool FindUser(const Person &user) const;

  void AddData(const Person &user);
  void DeleteAccount(const Person &user);
  void SaveData();
  void ChangePassword(const Person &user, const std::string &new_password);
  void CleanDatabase();

  Person &operator[](size_t index);
};