#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Menu.h"
#include <cstdlib>
#include <algorithm>

using namespace std;

class Employee;

struct Dish {
    string type, name;
    double cost;

    Dish() {
        cost = NULL;
    };
    Dish(string t, string n, double c) : type(t), name(n), cost(c) {};

    bool operator==(const Dish& other) const {
        return type == other.type &&
        name == other.name && 
        cost == other.cost;
    }

};

class RestMenu {
public:
	vector<Dish> dishes;
    vector<string> types;

    RestMenu() {
        types = { "garnish", "appetizer", "salad", "pizza", "first", "dessert" };

        ifstream file("menu.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string name, type;
            double price;
            getline(ss, name, ',');
            getline(ss, type, ',');
            ss >> price;
            type.erase(remove_if(type.begin(), type.end(), isspace), type.end());
            addDish(type, name, price);
        }
        file.close();
    };

	void addDish(string t, string n, double c) {
		Dish d(t, n, c);
		dishes.push_back(d);
	}

    vector<Dish> choiseDishes(bool isOne = false) {
        Menu m(20, types.size() + 2, 5, 20);
        vector<Dish> chosen;

        while (1) {
            m.setH(types.size() + 2);
            m.SetX(20);
            m.clearItems();
            for (string t : types) {
                m.AddElement(t.c_str());
            }
            m.AddElement("EXIT");

            int choiseT = m.Select();
            if (choiseT >= types.size()) break;

            m.SetX(41);
            m.clearItems();

            int count = count_if(dishes.begin(), dishes.end(), [choiseT, this](const Dish& dish) {
                return dish.type == types[choiseT];
            });
            m.setH(count + 1);
            for (Dish& d : dishes) {
                if (d.type == types[choiseT]) m.AddElement(d.name.c_str());
            }
            int chosenD = m.Select();

            int i = 0;
            for (Dish& d : dishes) {
                if (d.type == types[choiseT]) {
                    if (i == chosenD) { 
                        chosen.push_back(d);
                        if (isOne) return chosen;
                        break;
                    }
                    i++;
                }
            }
        }
        return chosen;

    }

    bool checkCost(char* inp) {
        if (inp[0] == 0) return false;
        for (int i = 0; inp[i] != 0; i++) {
            if (!isdigit(inp[i]))
                if (inp[i] == '.') continue; 
                return false;
        }
        return true;
    }

    void writeDish() {
        Dish d;

        Menu m(20, 10, 5, 20);
        m.addHeader(3, "Choise type of the dish");
        for (string t : types) {
            m.AddElement(t.c_str());
        }

        int t = m.Select();
        d.type = types[t];

        cls();
        Window w(30, 4, 5, 40);
        w.customInput("Dish name: ", 6);
        d.name = w.getInput();

        cls();
        do {
            w.Show();
            w.customInput("Cost: ", 6);
            try {
                d.cost = stod(w.getInput());
            }
            catch (const std::exception&) {}
        } while (d.cost == NULL);

        dishes.push_back(d);

        fstream file;
        file.open("menu.txt", (ios::out | ios::app));
        file << "\n" << d.name << ", " << d.type << ", " << d.cost;
        file.close();
    }

};