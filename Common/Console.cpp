#include <string>
#include <iostream>

#include <conio.h>

#include <Windows.h>

#include "Console.h"

void SetConsoleColor(ConsoleColor bgColor, ConsoleColor fgColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | fgColor);
}

char ConsoleReadKey()
{
	return (char)_getch();
}

void WaitForInput()
{
	_getch();
}

void ConsoleClear()
{
	ConsoleCmd("cls");
}

void ConsoleCmd(const std::string cmd)
{
	system(cmd.c_str());
}
