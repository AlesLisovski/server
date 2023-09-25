#include "Person.h"

Person::Person()
{
	login_ = "";
	password_ = "";
}

Person::Person(const std::string login, const std::string password)
{
	login_ = login;
	password_ = password;
}

void Person::SetLogin(const std::string& login)
{
	login_ = login;
}

void Person::SetPassword(const std::string& password)
{
	password_ = password;
}