#pragma once
#include "Person.h"
#include <vector>
#include <algorithm>

class database
{
	std::vector<Person> data_;

public:
	database();
	database(const std::string& file_name);
	std::vector<Person> get_data();
	int get_user_count();
	bool isempty();
	void add_data(const Person& user);
	void delete_account(const Person& user);
	bool find_user(const Person& user);
	void save_data(const std::string& file_name);
	void change_password(const Person& user, const std::string& new_password);
	void clean_database(const std::string file_name);
};