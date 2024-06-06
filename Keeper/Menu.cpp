#include "Menu.h"
#include<iostream>
#include <conio.h>

void Menu::customExit(int x, int y) {
	char num[50];

	GotoXY(x, y); cout << "Print (0) for exit: ";
	while (strcmp(num, "0") != 0) {
		GotoXY(x + 21, y); cin >> num;
	}
}

int Menu::Select() {
	if (GetCount() == 0) {
		active = -1;
		return active;
	}
	ShowCaret(false);
	active = 0;
	first = 0;

	while (1) {
		Show();

		if (active > first + GetHeight() - 1) {
			first = active - GetHeight() + 1;
		}
		else if (active < first) {
			first = active;
		}

		for (int i = indexY, j = first; i < GetHeight() && j < GetCount(); i++, j++) {
			if (j == active) {
				::SetColor(selectColor, selectBgColor);
				::GotoXY(x, y + 1 + i);
				for (int k = 0; k < GetWidth(); k++) {
					SetColor(color);
					SetBgColor(bgColor);
					std::cout << " ";
				}
			}

			else {
				::SetColor(color, bgColor);
			}
			DrawItem(i, j);
		}
		int key = _getch();
		if (key == 224) {
			key = _getch();
			if (key == 80 && active + 1 < GetCount()) { //down
				active++;
			}
			if (key == 72 && active > 0) { //up
				active--;
			}
			continue;
		}
		if (key == 27) {
			active = -1;
			break;
		}
		if (key == 13) {
			break;
		}

	}

	ShowCaret(true);

	return active;

}