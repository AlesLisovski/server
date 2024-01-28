#include "database.h"
#include <stdexcept>

Database::Database(const std::string &file_name) {
  database_ = file_name;

  if (sqlite3_open(file_name.c_str(), &db_) != SQLITE_OK) {
    std::cout << "Error opening database" << std::endl;
    return;
  }
}

int Database::Size() const {
  std::string sql = "SELECT COUNT(*) FROM USERS";
  int rows;
  sqlite3_exec(
      db_, sql.c_str(),
      [](void *data, int argc, char **argv, char **azColName) -> int {
        int *rows = static_cast<int *>(data);
        *rows = std::stoi(argv[0]);
        return 0;
      },
      &rows, nullptr);

  return rows;
}

bool Database::Empty() const { return Size() == 0; }

void Database::AddData(const Person &user) {
  std::string sql = "INSERT INTO USERS (Login, Password) VALUES ('" +
                    user.GetLogin() + "', '" + user.GetPassword() + "');";
  sqlite3_exec(db_, sql.c_str(), nullptr, 0, nullptr);
}

void Database::DeleteAccount(const Person &user) {
  std::string sql =
      "DELETE FROM USERS WHERE Login = '" + user.GetLogin() + "';";
  sqlite3_exec(db_, sql.c_str(), nullptr, 0, nullptr);
}

bool Database::FindUser(const Person &user) const {
  std::string sql = "SELECT * FROM Users WHERE Login = ? AND Password = ?";
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, 0);

  if (rc != SQLITE_OK) {
    std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db_)
              << std::endl;
    return (0);
  }

  std::string login = user.GetLogin();
  std::string password = user.GetPassword();

  sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

  bool result = sqlite3_step(stmt) == SQLITE_ROW;

  sqlite3_finalize(stmt);

  return result;
}

void Database::CloseDatabase() { sqlite3_close(db_); }

void Database::ChangePassword(const Person &user,
                              const std::string &new_password) {
  std::string sql = "UPDATE USERS SET Password = '" + new_password +
                    "' WHERE Login = '" + user.GetLogin() + "';";
  sqlite3_exec(db_, sql.c_str(), nullptr, 0, nullptr);
}
