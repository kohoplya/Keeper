#pragma once
#include <string>

class Window
{
protected: friend class Keyboard;
	int x, y, width, height;
	int indexY = y;
	int color, bgColor, borderColor;
	char buf[6];
	int active;
	int selectColor, selectBgColor;
	int first;
	char* input;

public:
	Window();
	Window(Window& win);
	Window(int width, int height, int y = 0, int x = 20);
	Window(const Window& other) {
		this->height = other.height;
		this->width = other.width;
		this->y = other.y;
		this->x = other.x;
	}
	~Window() { delete[] input; }
	void createWin();
	void customInput(const char* text = "", int _y = 1);
	char* getInput() { return input; };

	void Move(int _x, int _y);
	void SetColor(int c);
	void SetColors(int c, int b) { selectColor = c; selectBgColor = b; }
	void SetBgColor(int c);
	void SetSelectColor(int c) { selectColor = c; }
	void SetSelectBgColor(int c) { selectBgColor = c; };
	void SetBorderColor(int c);
	void SetX(int x) { this->x = x; }
	int getMiddle(int len) {
		int middle = width / 2 - len / 2;
		return middle + x;
	}

	int GetX() { return x; }
	int GetY() { return y; }

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	void SetWidth(int w);
	void SetHeight(int h);

	virtual void Show();
	int Select();
	void DrawItem(int row, int index);
	Window& Write(int _x, int _y, const char* str);
	void writeText(int _y, const char* str, int _x = 0);
	void writeText(int _y, std::string str, int _x = 0);
	int GetSelect() { return active; };

	void clear();
};