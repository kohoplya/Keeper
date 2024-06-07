#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include "Employee.h"
#include "Window.h"
#include "Menu.h"
#include "Keyboard.h"
#include "Order.h"
#include "Dish.h"
#include <conio.h>
#include <string>

using namespace std;

struct Cashier {
	double cash, card;

	void printCashier(int x, int y) {
		Window cashierWin(20, 3, y, x);

		stringstream ss;
		ss << "Cash: " << cash;
		cashierWin.writeText(y + 1, ss.str());
		ss.str("");

		ss << "Card: " << card;
		cashierWin.writeText(y + 2, ss.str());

	}
};

class Keeper
{
public:
	vector<Employee> employees;
	RestMenu menu;
	Cashier cashier;
	Employee* current;
	vector<Order*> orders;

	Keeper() {
		fstream file;
		file.open("employeers.dat", (ios::in | ios::binary | ios::ate));
		int size = (file.tellg() / sizeof(Employee));
		file.seekg(0, ios::beg);
		Employee* e = new Employee[size];
		for (int i = 0; i < size; i++) {
			file.read((char*)&e[i], sizeof(Employee));
			employees.push_back(e[i]);
		}
		file.close();

		Employee adminM("Admin", "Ivanov", "7777", 1);
		employees.push_back(adminM);

	};

	void login() {
		Window w(26, 4, 5, 40);
		string s("Enter password");
		w.writeText(7, s, w.getMiddle(s.length()));
	}

	Employee* searchEmployee(char* pass) {
		for (Employee& e : employees)
			if (e.password == pass)
				return &e;

		return nullptr;
	}

	string enterData(int x, int y, string desc, int len, bool isNum = false, bool isLogin = false) {
		Window w(len + desc.length() + 1, 4, y, x);
		string input;
		bool isUniq;

		do {
			w.Show();
			isUniq = true;
			w.customInput(desc.c_str(), w.GetY()+1);
			input = w.getInput();

			if (isNum) {
				try {
					stod(w.getInput());
				}
				catch (const std::exception&) {}
			}

			if (isLogin) {
				for (Employee& e : employees) {
					if (e.login == input) {
						isUniq = false;
						break;
					}
				}
			}
		} while (input.empty() || (isLogin && !isUniq));

		return input;
	}

	int tableSelect() {
		Window header(33, 3, 2, 40);
		header.writeText(3, "Choise the number of table");
		Keyboard kb(40, 6, 7, 41, 20, true, 5, true);
		int num = kb.Select();

		return num;
	}

	void mainMenu() {
		Menu menuWin(25, 9, 6, 10);

		menuWin.AddElement("Create order");
		menuWin.AddElement("Edit order");
		menuWin.AddElement("Close order");

		if (current->isAdmin) {
			menuWin.AddElement("Add employee");
			menuWin.AddElement("Change waiter");
			menuWin.AddElement("Add dish");
			menuWin.AddElement("Remove dish");
		}
		menuWin.AddElement("EXIT");

		menuWin.setH(menuWin.GetCount() + 1);
		stringstream ss;
		ss << "Welcome, " << current->login << " " << current->lastname << "!" << endl;
		menuWin.addHeader(3, ss.str());
		ss.str("");
		if (current->isAdmin) ss << "(Administrator panel)";
		else ss << "(Waiter's panel)";
		menuWin.addFooter(3, ss.str());

		while (1) {
			int choise = menuWin.Select();

			if (choise == 0) createOrder();
			if (choise == 1) editOrder();
			if (choise == 2) closeOrder();
			if (current->isAdmin) {
				if (choise == 3) addEmployee();
				if (choise == 4) changeWaiter();
				if (choise == 5) menu.writeDish();
				if (choise == 6) removeDish();
			}
			if (choise == 7 || (!current->isAdmin && choise == 3)) {
				choise = -1;
			}
			if (choise == -1) {
				cls();
				return;
			}
		}
	}

	Menu& printOrders(Menu& menuWin) {
		stringstream ss;

		vector<Order*> empOrders;

		if (current->isAdmin) empOrders = orders;
		else {
			for (Order* o : orders) {
				if (o->emp->login == current->login) empOrders.push_back(o);
			}
		}

		if (empOrders.size() == 0) {
			ss << "No orders is here";
			menuWin.addFooter(3, ss.str());
			ss.str("");

			menuWin.AddElement("Exit");
			menuWin.Select();
			menuWin.clearItems();
			return menuWin;
		}

		menuWin.setH(empOrders.size() + 2);

		for (Order* o : empOrders) {
			ss << "table: " << o->table << " -> " << o->emp->login;
			menuWin.AddElement(ss.str().c_str());
			ss.str("");
		}
		menuWin.AddElement("Exit");

		return menuWin;
	}

	int choiceEmp(int x, int y) {
		Menu menuWin(19, 8, y, x);
		stringstream ss;

		for (Employee& e : employees) {
			ss << e.login << " " << e.lastname;
			menuWin.AddElement(ss.str().c_str());
			ss.str("");
		}
		return menuWin.Select();
	}

	void createOrder() {
		clsXY(LightGray, 37, 2, 150, 22);
		int table = tableSelect();
		Order* order = new Order(current);
		order->table = table;
		clsXY(LightGray, 37, 2, 90, 22);

		order->dishes = menu.choiseDishes();
		orders.push_back(order);
		clsXY(LightGray, 37, 2, 90, 22);

		Menu menuWin(19, 8, 9, 40);
		order->printOrder(40, 2);
		menuWin.setH(3);
		menuWin.setW(10);
		menuWin.Move(67, 2);

		menuWin.AddElement("EXIT");
		menuWin.Select();
	}

	void editOrder() {
		clsXY(LightGray, 37, 2, 100, 22);
		Menu menuWin(19, 3, 2, 40);
		Menu editWin(25, 3, 2, 61);
		editWin.AddElement("Edit").AddElement("Exit");

		vector <Dish> chosen;
		printOrders(menuWin);
		
		int lastChoice = 0;
		while (1) {
			int choice = menuWin.Select();
			if (choice >= orders.size()) break;

			clsXY(LightGray, 84, 9, 150, 9 + (orders[lastChoice]->dishes.size() + 10));
			orders[choice]->printOrder(61, 2);
			editWin.Move(61, GetCurrentY() + 2);
			int editChoice = editWin.Select();
			lastChoice = choice;
			clsXY(LightGray, 37, 2, 150, 22);

			while (editChoice == 0) {
				chosen = menu.choiseDishes(true);
				if (chosen.empty()) { 
					break; 
				}
				orders[choice]->dishes.push_back(chosen[0]);
				orders[choice]->printOrder(84, 2);
				editWin.Move(84, GetCurrentY() + 2);
				int editChoise = editWin.Select();
				if (editChoise == 1) {
					clsXY(LightGray, 37, 2, 60, 22);
					break;
				}
			}
		}

	}

	void closeOrder() {
		clsXY(LightGray, 37, 2, 150, 22);
		Menu menuWin(20, 3, 6, 40);
		Menu closeWin(25, 4, 2, 67);

		closeWin.AddElement("Cash").AddElement("Card").AddElement("Exit");

		int lastSize = 0;
		while (1) {
			cashier.printCashier(40, 2);
			menuWin.clearItems();
			printOrders(menuWin);
			int choice = menuWin.Select();
			if (choice >= orders.size()) break;
			clsXY(LightGray, 66, 2, 120, 2 + lastSize);
			orders[choice]->printOrder(67, 2);
			closeWin.Move(67, GetCurrentY() + 2);

			int closeChoice = closeWin.Select();
			lastSize = orders[choice]->dishes.size() + 12;

			while (closeChoice < 2) {
				if (closeChoice == 0) {
					cashier.cash += orders[choice]->getTotal();
				} else cashier.card += orders[choice]->getTotal();
				clsXY(LightGray, 37, 6, 60, 6 + orders.size() + 2);
				orders.erase(orders.begin() + choice);
				break;
			}
		}
	}

	void addEmployee() {
		clsXY(LightGray, 37, 2, 150, 22);
		string login, lastname, password;
		bool isAdmin = 0;

		login = enterData(40, 2, "Enter login: ", 20, false, true);
		lastname = enterData(40, 2, "Enter lastname: ", 20, false);
		password = enterData(40, 2, "Enter password: ", 4, true);
		clsXY(LightGray, 37, 2, 80, 10);

		Menu m(10, 3, 6, 40);
		m.addHeader(3, "Is admin?");
		m.AddElement("yes").AddElement("no");
		int choice = m.Select();
		if (choice == 0) isAdmin = 1;

		Employee e(login, lastname, password, isAdmin);
		employees.push_back(e);
		fstream file;
		file.open("employeers.dat", (ios::out | ios::app | ios::binary));
		file.write((char*)&e, sizeof(e));
		file.close();
	}

	void changeWaiter() {
		clsXY(LightGray, 37, 2, 150, 22);
		Menu menuWin(19, 3, 2, 40);
		Menu changeWin(25, 3, 2, 61);
		changeWin.AddElement("Change").AddElement("Exit");

		int lastSize = 0;
		while (1) {
			menuWin.clearItems();
			clsXY(LightGray, 37, 2, 60, orders.size() + 5);
			printOrders(menuWin);
			int choice = menuWin.Select();
			if (choice >= orders.size()) break;
			clsXY(LightGray, 60, 2, 120, 2 + lastSize);
			orders[choice]->printOrder(63, 2);
			lastSize = orders[choice]->dishes.size() + 12;

			changeWin.Move(63, GetCurrentY() + 2);
			int changeChoice = changeWin.Select();
			if (changeChoice != 1) {
				int choiceE = choiceEmp(92, 2);
				orders[choice]->emp = &employees[choiceE];
			}
		}
	}

	void removeDish() {
		clsXY(LightGray, 37, 2, 150, 22);
		Menu menuWin(19, 8, 2, 40);

		vector<Dish> deleted;
		deleted = menu.choiseDishes();

		for (Dish& d : deleted) {
			for (int i = 0; i < menu.dishes.size(); i++) {
				if (d == menu.dishes[i]) menu.dishes.erase(menu.dishes.begin() + i);
			}
		}
	}
};