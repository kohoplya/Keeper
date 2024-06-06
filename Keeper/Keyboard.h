#pragma once
#include <vector>
#include "Window.h"
#include <sstream>
#include "Menu.h"
#include <conio.h>
#include <iostream>

using namespace std;

class Key : public Window {
public:
	int t;

	Key(int width, int height, int y = 0, int x = 20,int i = 0, bool isTen = false) : Window(width, height, y, x) {
		if (i == 9 && !isTen) t = 0;
		else t = i + 1;
	};
	void Show() {
		Window::Show();
		writeText(GetY() + 1, to_string(t));
	}
};

class Keyboard : public Menu
{
public:
	vector<Key*> keys;
	int keyW = 5;
	int keyY = 3;
	int count;
	int active;
	bool isTen;
	bool isOne;
	int collum;
	int color = LightGray;
	int bgcolor = Black;
	int inputY;
	int inputX;
	char input[5];

	Keyboard(int x = 1, int y = 0, int inputY = 0, int inputX = 0, int count = 10, bool isTen = false, int collum = 3, bool isOne = false) {
		this->inputX = inputX;
		this->inputY = inputY;
		this->collum = collum;
		this->isOne = isOne;

		int _x = 0;
		for (int i = 0; i < count; i++) {
			keys.push_back(new Key(keyW, keyY, y, x + ((keyW + 2) * _x), i, isTen));
			if ((i + 1) % collum == 0) {
				y += keyY + 1;
				_x = 0;
			}
			else _x++;
		}
	};

	~Keyboard() {
		for (Key* k : keys) {
			delete k;
		}

	};

	void show() {
		for (int i = 0; i < keys.size(); i++) {
			keys[i]->Show();
			keys[i]->writeText(keys[i]->GetY() + 1, to_string(keys[i]->t));
		}

	}

	int Select() {
		GotoXY(inputX, inputY);
		cout << string(4, ' ');
		ShowCaret(false);
		active = 0;
		show();

		while (1) {
			if (strlen(input) >= 4) return active;
			int key = _getch();

			switch (key) {
			case 72: // up
				active = (active - collum + keys.size()) % keys.size();
				break;
			case 80: // down
				active = (active + collum) % keys.size();
				break;
			case 75: // left
				active = (active - 1 + keys.size()) % keys.size();
				break;
			case 77: // right
				active = (active + 1) % keys.size();
				break;
			case 13: // enter
				if (isOne) return active + 1;
				GotoXY(inputX, inputY);
				cout << keys[active]->t;

				snprintf(input + strlen(input), sizeof(input) - strlen(input), "%d", keys[active]->t);
				inputX++;
			default:
				break;
			}

			for (int i = 0; i < keys.size(); i++) {
				if (i == active) {
					keys[i]->color = color;
					keys[i]->bgColor = bgcolor;
				}
				else {
					keys[i]->color = bgcolor;
					keys[i]->bgColor = color;
				};
				keys[i]->Show();
				::SetColor(bgcolor, color);
			}
		}

		return active;
	}

};

