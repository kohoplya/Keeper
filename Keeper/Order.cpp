#include "Order.h"
#include "Employee.h"
#include "Dish.h"
#include <string>
#include <conio.h>
#include <iomanip>

void Order::printOrder(int x, int y) {
	Window w(25, dishes.size() + 6, y, x);
	string line(w.GetWidth(), '=');
	stringstream ss;

	ss << emp->login << " " << emp->lastname;
	w.writeText(y + 1, ss.str(), w.getMiddle(ss.str().length()));

	ss.str("");
	ss << "table: " << table;
	w.writeText(y + 2, ss.str(),w.getMiddle(ss.str().length()));
	w.writeText(y + 3, line, w.GetX());

	int i = 0;
	for (Dish& d : dishes) {
		w.writeText(y + 4 + i, d.name);
		ss.str("");
		ss << fixed << setprecision(2) << d.cost;
		w.writeText(y + 4 + i, ss.str(), w.GetX() + w.GetWidth() - 5);
		i++;
	}
	w.writeText(y + w.GetHeight() - 2, line, w.GetX());

	ss.str("");
	ss << "Final cost: " << getTotal();
	w.writeText(y + w.GetHeight() - 1, ss.str(), w.GetX());

}