#pragma once
#include "../../utilities/csgo.hpp"

#pragma region logging_console_colors
#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)
#pragma endregion

namespace console {
	void initialize(const char* title);
	void release();

	inline std::uint8_t		wConsoleColor = FOREGROUND_WHITE;

	template <typename ... Args>
	void log(char const* const title,char const* const format, Args const& ... args) {
		time_t t;
		t = time(NULL);
		struct tm tm = *localtime(&t);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_GREEN);
		printf(xor("[%2d:%2d:%2d] "), tm.tm_hour, tm.tm_min, tm.tm_sec);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_CYAN);
		printf(title);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE);
		printf(xor (" --> "));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wConsoleColor);
		printf(format, args ...);
		printf("\n");
	}

	inline const void push_color(const std::uint8_t wColor)
	{
		wConsoleColor = wColor;
	}

	/* reset console color */
	inline const void pop_color()
	{
		wConsoleColor = FOREGROUND_WHITE;
	}
}