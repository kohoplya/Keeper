#pragma once
#include "Window.h"
#include "fun_console.h"
#include <vector>
#include <string>

using namespace std;

class Menu : public Window {
protected:
	int active;
	int selectColor, selectBgColor;
	int first;

	vector<string> items;
public:
	Menu() : Window() {
		Move(20, 5);
		SetWidth(44);
		SetHeight(8);
		SetSelectColor(0);
		SetSelectBgColor(15);
		active = -1;
	}

	Menu(int width, int height, int y = 0, int x = 20) {
		createWin();

		Move(x, y);
		SetHeight(height);
		SetWidth(width);
		SetSelectColor(0);
		SetSelectBgColor(15);
		active = -1;

	};

	Menu(const Menu& other) {
		this->height = other.height;
		this->width = other.width;
		this->y = other.y;
		this->x = other.x;
		this->items = other.items;
	}

	void customExit(int x, int y);

	void SetSelectColor(int c) { selectColor = c; }
	void SetSelectBgColor(int c) { selectBgColor = c; };
	int GetSelect() { return active; }
	int Select();
	Menu& AddElement(const char* m, bool isSelectable = true) {
		items.push_back(m);
		return *this;
	}

	Menu& addHeader(int _y, string str) {
		Window win(this->width, _y, this->y - _y - 1, this->x);
		win.writeText((this->y - _y), str);
		return *this;

	}

	Menu& addFooter(int _y, string str) {
		Window win(this->width, _y, this->y + this->height + 1, this->x);
		win.writeText((this->y + this->height + 2), str);
		return *this;
	}

	void setW(int w) { this->width = w; }
	void setH(int h) { this->height = h; }
	void clearItems() {
		items.clear();
	}

	int GetCount() {
		return items.size();
	}
	void DrawItem(int row, int index) {
		Write(0, row, items[index].c_str());
	}

};