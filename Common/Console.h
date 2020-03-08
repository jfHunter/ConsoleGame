#pragma once

#include <iostream>
#include <string>

#include <stdint.h>

using std::string;

typedef enum {
	CONSOLE_COLOR_BLACK,
	CONSOLE_COLOR_BLUE,
	CONSOLE_COLOR_GREEN,
	CONSOLE_COLOR_CYAN,
	CONSOLE_COLOR_RED,
	CONSOLE_COLOR_MAGENTA,
	CONSOLE_COLOR_BROWN,
	CONSOLE_COLOR_WHITE,
	CONSOLE_COLOR_GRAY,
	CONSOLE_COLOR_LIGHT_BLUE,
	CONSOLE_COLOR_LIGHT_GREEN,
	CONSOLE_COLOR_LIGHT_CYAN,
	CONSOLE_COLOR_LIGHT_RED,
	CONSOLE_COLOR_LIGHT_MAGENTA,
	CONSOLE_COLOR_YELLOW,
	CONSOLE_COLOR_LIGHT_WHITE
} ConsoleColor;

void SetConsoleColor(ConsoleColor bgColor, ConsoleColor fgColor);

char ConsoleReadKey();
void WaitForInput();
void ConsoleClear();
void ConsoleCmd(const string cmd);