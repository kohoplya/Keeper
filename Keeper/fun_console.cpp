#include <windows.h>
#include "fun_console.h"
#include <iostream>

void GotoXY(short x, short y) {
	COORD coord = { x,  y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void cls(int c, int b)
{
	SetColor(c, b);
	system("cls");
}

void clsXY(short bgcolor, int x1, int y1, int x2, int y2) {
	SetColor(bgcolor, bgcolor);
	for (int i = 0; i <= x2 - x1; i++) {
		for (int j = 0; j <= y2 - y1; j++){
			GotoXY(x1 + i, y1 + j);
			std::cout << " ";
		}
	}
}

void SetColor(short textcolor, short bgcolor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor << 4) | textcolor));
}

void SetSize(int X, int Y)
{
	COORD coord = { (short)X, (short)Y };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


int GetRows()
{
	CONSOLE_SCREEN_BUFFER_INFO c;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
		return 0;
	return c.dwSize.Y;
}

int GetCols()
{
	CONSOLE_SCREEN_BUFFER_INFO c;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
		return 0;
	return c.dwSize.X;
}

int GetCurrentY()
{
	CONSOLE_SCREEN_BUFFER_INFO c;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
		return 0;
	return c.dwCursorPosition.Y;
}

int GetCurrentX()
{
	CONSOLE_SCREEN_BUFFER_INFO c;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
		return 0;
	return c.dwCursorPosition.X;
}

bool GetCaret() {
	CONSOLE_CURSOR_INFO ci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
	return ci.bVisible;
}

void ShowCaret(bool show)
{
	CONSOLE_CURSOR_INFO ci;
	ci.bVisible = show;
	ci.dwSize = 100;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}


