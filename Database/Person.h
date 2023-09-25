#pragma once
#include <string>
#include <fstream>

struct Person
{
private:
	std::string login_;
	std::string password_;

public:

	Person();
	Person(const std::string login, const std::string password);
	void SetLogin(const std::string& login);
	void SetPassword(const std::string& password);
	std::string GetLogin() { return login_; }
	std::string GetPassword() { return password_; }

	bool operator <(const Person& user2)
	{
		return login_[0] < user2.login_[0];
	}

	bool operator ==(const Person& user2)
	{
		return login_ == user2.login_;
	}

	friend std::istream& operator >>(std::istream& in, Person& person)
	{
		std::string login, password;
		in >> login >> password;
		person.SetLogin(login);
		person.SetPassword(password);

		return in;
	}

	friend std::ostream& operator << (std::ostream& os, Person& person)
	{
		return os << person.GetLogin() << " " << person.GetPassword();
	}
};