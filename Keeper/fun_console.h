#pragma once
#ifndef _FUN_CONSOLE
#define _FUN_CONSOLE
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void GotoXY(short x, short y);
void cls(int c = 0, int b = 7);
void SetColor(short textcolor, short bgcolor);
void clsXY(short bgcolor, int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
void SetSize(int X, int Y);
int GetRows();
int GetCols();
int GetCurrentY();
int GetCurrentX();
bool GetCaret();
void ShowCaret(bool show = true);

#endif
