#pragma once
#include "person/person.h"
#include "sqlite3/sqlite3.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class Database {
  std::string database_;
  sqlite3 *db_;

public:
  Database(const std::string &file_name);

  int Size() const;
  bool Empty() const;
  bool FindUser(const Person &user) const;

  void AddData(const Person &user);
  void DeleteAccount(const Person &user);
  void CloseDatabase();
  void ChangePassword(const Person &user, const std::string &new_password);

  Person &operator[](size_t index);
};