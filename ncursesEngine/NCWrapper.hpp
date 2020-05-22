#ifndef NCWRAPPER_INCLUDE
#define NCWRAPPER_INCLUDE

#include <ncurses.h>

// Enumeration of colour pairs used for printing on stdscr.
// First colour is text, second is background.
enum ColourPairs
	{
		// every next color is ++1 after 30
		PAIR_BLUE_BLUE = 30,
		PAIR_RED_BLACK,
		PAIR_YELLOW_BLACK,
		PAIR_BLUE_BLACK,
		PAIR_GREEN_BLACK,
		PAIR_WHITE_BLACK,
		PAIR_RED_WHITE,
		PAIR_BLACK_WHITE
	};

// Wrapped-around interactions with low-level
// ncurses functions.

// Point of this is to abstract away direct ncurses use.
namespace NCWRAPPER
	{
		// Initializes ncurses mode.
		void init();

		/// Exits ncurses mode
		void exit();

	}
	
#endif 