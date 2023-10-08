#include "database.h"
#include <stdexcept>

Database::Database(const std::string &file_name) {
  database_ = file_name;
  std::ifstream in(file_name);

  while (!in.eof()) {
    Person user;
    in >> user;
    data_.push_back(user);
  }

  in.close();
}

int Database::Size() const { return data_.size(); }

bool Database::Empty() const { return data_.empty(); }

void Database::AddData(const Person &user) {
  if (!FindUser(user)) {
    data_.push_back(user);
  }
}

void Database::DeleteAccount(const Person &user) {
  auto result = std::find(data_.begin(), data_.end(), user);

  if (result != data_.end()) {
    data_.erase(result);
  }
}

bool Database::FindUser(const Person &user) const {
  auto result = std::find(data_.begin(), data_.end(), user);
  return data_.end() != result;
}

void Database::SaveData() {
  std::ofstream out(database_);

  if (out.is_open()) {
    int size = data_.size();

    for (int i = 0; i < size - 1; i++) {
      out << data_[i] << "\n";
    }

    if (size != 0) {
      out << data_[size - 1];
    }
  }

  out.close();
}

void Database::ChangePassword(const Person &user,
                              const std::string &new_password) {
  auto result = std::find(data_.begin(), data_.end(), user);

  if (result != data_.end()) {
    int it = std::distance(data_.begin(), result);
    data_[it].SetPassword(new_password);
  }
}

void Database::CleanDatabase() {
  std::ofstream out(database_, std::ios::out);
  data_.clear();
  out.close();
}

Person &Database::operator[](size_t index) {
  if (index < 0 || index >= data_.size()) {
    throw std::out_of_range("Index out of range");
  }
  return data_[index];
}