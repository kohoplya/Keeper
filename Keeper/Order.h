#pragma once
#include "Dish.h"
#include "Employee.h"
#include <vector>
#include <sstream>

using namespace std;

class Employee;

class Order
{
public:
	Employee* emp;
	vector<Dish> dishes;
	int table;
	double total;

	Order(Employee* e) : emp(e) {};

	void printOrder(int x, int y);
	double getTotal() {
		total = 0;
		for (Dish& d : dishes) {
			total += d.cost;
		}
		return total;
	}

	bool operator==(const Order& other) const {
		return
			table == other.table &&
			total == other.total &&
			dishes == other.dishes;
	}

};