#include "NCWrapper.hpp"

#include <sys/select.h>	// select()
#include <unistd.h>	
#include <locale.h>



void NCWRAPPER::init () {
	setlocale(LC_CTYPE, "");
	initscr();
	curs_set(0);
	cbreak();
	noecho();
	nodelay(stdscr, true);
	keypad(stdscr, true);

	start_color();
	init_pair(PAIR_RED_BLACK, COLOR_RED, COLOR_BLACK);
	init_pair(PAIR_BLUE_BLUE, COLOR_BLUE, COLOR_BLUE);
	init_pair(PAIR_YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
	init_pair(PAIR_YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
	init_pair(PAIR_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
	init_pair(PAIR_GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
	init_pair(PAIR_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(PAIR_RED_WHITE, COLOR_RED, COLOR_WHITE);
	init_pair(PAIR_BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

	refresh();
}

void NCWRAPPER::exit () {
	erase();
	refresh();
	endwin();
}