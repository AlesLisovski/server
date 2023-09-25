#include "database.h"
#include <stdexcept>

database::database()
{

}

database::database(const std::string& file_name)
{
	std::ifstream in(file_name);
	int n;
	std::string login, password;
	Person user;

	if (in.is_open())
	{
		if (in.peek() != EOF)
		{
			in >> n;

			for (int i = 0; i < n; i++)
			{
				in >> user;
				data_.push_back(user);
			}
		}
	}

	in.close();
}

std::vector<Person> database::get_data()
{
	return data_;
}

int database::get_user_count()
{
	return data_.size();
}

bool database::isempty()
{
	return data_.empty();
}

void database::add_data(const Person& user)
{
	if (!find_user(user))
	{
		data_.push_back(user);
		std::sort(data_.begin(), data_.end());
	}
}

void database::delete_account(const Person& user)
{
	if (find_user(user))
	{
		auto result = std::find(data_.begin(), data_.end(), user);
		data_.erase(result);
	}
	else
	{
		throw std::runtime_error("User not found");
	}
}

bool database::find_user(const Person& user)
{
	auto result = std::find(data_.begin(), data_.end(), user);
	return data_.end() != result;
}

void database::save_data(const std::string& file_name)
{
	std::ofstream out(file_name);

	if (out.is_open())
	{
		int size = data_.size();
		out << size << "\n";

		for (int i = 0; i < size - 1; i++)
		{
			out << data_[i] << "\n";
		}
		out << data_[size - 1];
	}
}

void database::change_password(const Person& user, const std::string& new_password)
{
	if (find_user(user))
	{
		auto result = std::find(data_.begin(), data_.end(), user);
		int it = std::distance(data_.begin(), result);
		data_[it].SetPassword(new_password);
	}
	else
	{
		throw std::runtime_error("User not found");
	}
}

void database::clean_database(const std::string file_name)
{
	std::ofstream out(file_name, std::ios::out);
	data_.clear();
	out.close();
}
