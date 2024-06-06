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

	Keeper() {
		fstream file;
		file.open("employeers.dat", (ios::in | ios::binary | ios::ate));
		int size = (file.tellg() / sizeof(Employee));
		if (!size || !file) { return; }
		file.seekg(0, ios::beg);
		Employee* e = new Employee[size];
		for (int i = 0; i < size; i++) {
			file.read((char*)&e[i], sizeof(Employee));
			employees.push_back(e[i]);
		}
		file.close();

		Employee adminM("Max", "Fun", "7777", 1);
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

	string enterData(string desc, int len, bool isNum = false, bool isLogin = false) {
		cls();
		Window w(len + desc.length() + 1, 4, 9, 40);
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
		Window header(33, 4, 4, 40);
		header.writeText(6, "Choise the number of table");
		Keyboard kb(40, 9, 7, 41, 20, true, 5, true);
		int num = kb.Select();

		return num;
	}

	void mainMenu() {
		Menu menuWin(19, 9, 9, 40);
		stringstream ss;
		ss << "Welcome " << current->login << " " << current->lastname << endl;
		menuWin.addHeader(3, ss.str());

		menuWin.AddElement("Create order");
		menuWin.AddElement("Edit order");
		menuWin.AddElement("Close order");
		menuWin.AddElement("Add employee");
		menuWin.AddElement("Change waiter");
		menuWin.AddElement("Add dish");
		menuWin.AddElement("Remove dish");
		menuWin.AddElement("EXIT");


		while (1) {
			cls();
			menuWin.addHeader(3, ss.str());

			int choise = menuWin.Select();

			if (choise == -1) { 
				cls();
				return; 
			}
			if (choise == 0) createOrder();
			if (choise == 1) editOrder();
			if (choise == 2) closeOrder();
			if (choise == 3) addEmployee();
			if (choise == 4) changeWaiter();
			if (choise == 5) menu.writeDish();
			if (choise == 7) { 
				cls();
				return; 
			}

		}
	}

	Menu& printOrders(Menu& menuWin) {
		stringstream ss;

		if (current->orders.size() == 0) {
			ss << "No orders is here";
			menuWin.addHeader(3, ss.str());
			ss.str("");

			menuWin.AddElement("Exit");
			menuWin.Select();
			menuWin.clearItems();
			return menuWin;
		}

		menuWin.setH(current->orders.size() + 2);

		for (Order& o : current->orders) {
			ss << "table: " << o.table << " -> " << o.emp->login;
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
		Menu menuWin(19, 8, 9, 40);

		int table = tableSelect();
		Order order(current);
		order.table = table;
		cls();

		order.dishes = menu.choiseDishes();
		cls();
		current->orders.push_back(order);

		for (Employee& e : employees) {
			if (e.isAdmin && &e != current) e.orders.push_back(order);
		}

		order.printOrder(40, 5);
		menuWin.setH(3);
		menuWin.setW(10);
		menuWin.Move(67, 5);

		menuWin.AddElement("EXIT");
		menuWin.Select();
		cls();
	}

	void editOrder() {
		cls();
		Menu menuWin(19, 3, 9, 40);
		Menu editWin(25, 3, 9, 67);
		editWin.AddElement("Edit").AddElement("Exit");

		vector <Dish> chosen;
		printOrders(menuWin);
		
		int lastChoice = 0;
		while (1) {
			int choice = menuWin.Select();
			if (choice >= current->orders.size()) break;

			clsXY(LightGray, 66, 9, 100, 9 + (current->orders[lastChoice].dishes.size() + 10));
			current->orders[choice].printOrder(67, 9);
			editWin.Move(67, GetCurrentY() + 2);
			int editChoice = editWin.Select();
			lastChoice = choice;
			cls();

			while (editChoice == 0) {
				chosen = menu.choiseDishes(true);
				if (chosen.empty()) { 
					cls();  
					break; 
				}
				current->orders[choice].dishes.push_back(chosen[0]);
				current->orders[choice].printOrder(67, 5);
				editWin.Move(67, GetCurrentY() + 2);
				int editChoise = editWin.Select();
				if (editChoise == 1) { 
					cls();
					break;
				}
			}
		}

	}

	void closeOrder() {
		cls();
		Menu menuWin(19, 3, 9, 40);
		Menu closeWin(25, 4, 9, 67);

		closeWin.AddElement("Cash").AddElement("Card").AddElement("Exit");

		int lastChoice = 0;
		while (1) {
			cls();
			cashier.printCashier(12, 9);
			menuWin.clearItems();
			printOrders(menuWin);
			int choice = menuWin.Select();
			if (choice >= current->orders.size()) break;
			clsXY(LightGray, 66, 9, 100, 9 + (current->orders[lastChoice].dishes.size() + 10));
			current->orders[choice].printOrder(67, 9);
			closeWin.Move(67, GetCurrentY() + 2);

			int closeChoice = closeWin.Select();
			lastChoice = choice;

			while (closeChoice < 2) {
				if (closeChoice == 0) {
					cashier.cash += current->orders[choice].getTotal();
				} else cashier.card += current->orders[choice].getTotal();
				current->orders.erase(current->orders.begin() + choice);
				break;
			}
		}
	}

	void addEmployee() {
		string login, lastname, password;
		bool isAdmin = 0;

		login = enterData("Enter name: ", 20, false, true);
		lastname = enterData("Enter lastname: ", 25, false);
		password = enterData("Enter password: ", 4, true);
		cls();
		Menu m(10, 3, 9, 40);
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
		Menu menuWin(19, 3, 9, 40);

		while (1) {
			cls();
			menuWin.clearItems();
			printOrders(menuWin);
			int choice = menuWin.Select();
			if (choice >= current->orders.size()) break;
			int choiceE = choiceEmp(61, 9);
			Employee& choicedEmp = employees[choiceE];
			Employee* choicedOrderEmp = nullptr;
			for (Employee& e : employees) {
				if (current->orders[choice].emp->login == e.login) choicedOrderEmp = &e;
			}
			current->orders[choice].emp = &choicedEmp;
			choicedEmp.orders.push_back(current->orders[choice]);
			for (int i = 0; i < choicedEmp.orders.size(); i++) {
				if (current->orders[choice] == choicedEmp.orders[i]) {
					choicedEmp.orders.erase(choicedEmp.orders.begin() + i);
				}
			}
			for (int i = 0; i < choicedOrderEmp->orders.size(); i++) {
				if (current->orders[choice] == choicedOrderEmp->orders[i]) {
					choicedOrderEmp->orders.erase(choicedOrderEmp->orders.begin() + i);
				}
			}
		}
	}
};