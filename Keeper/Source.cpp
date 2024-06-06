#include <iostream>
#include <string>
#include "Dish.h"
#include "Window.h"
#include "Keyboard.h"
#include "Keeper.h"

#include <conio.h>

using namespace std;

int main() {
	cls();

	Keeper keep;
	Keyboard kb(40, 10, 8, 41,10, false, 4);
	keep.login();

	while (1) {
		keep.login();
		kb.inputX = 41;
		kb.Select();

		while (keep.searchEmployee(kb.input) == nullptr) {
			memset(kb.input, 0, sizeof(kb.input));
			Window error(20, 4, 5, 70);
			error.SetBorderColor(Red);
			error.SetColor(Red);
			error.Show();
			error.writeText(7, "UNCORRECT PASSWORD!!");
			kb.inputX = 41;
			kb.Select();
		}
		keep.current = (keep.searchEmployee(kb.input));
		cls();
		keep.mainMenu();
		kb.input[0] = 0;
	}

	return false;
}