#pragma once
#include <string>
#include <vector>
#include "Order.h"

using namespace std;

class Order;

class Employee
{
public:
	string login, lastname;
	string password;
	bool isAdmin = 0;

	Employee() {};
	Employee(string n, string l, string p, bool i) : login(n), lastname(l), password(p), isAdmin(i) {};

	bool operator==(const Employee& other) const {
		return login == other.login &&
			lastname == other.lastname &&
			password == other.password &&
			isAdmin == other.isAdmin;
	}
};