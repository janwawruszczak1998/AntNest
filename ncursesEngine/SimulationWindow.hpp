#ifndef SIMULATION_WINDOW_INCLUDE
#define SIMULATION_WINDOW_INCLUDE

//#include <Engine/CNCurses.h>

#include <string>
#include <vector>
#include <ncurses.h>


/// Alias for filling whole parent window with child window.
#define FILL_WINDOW 0


/// A wrap around class for ncurses WINDOW. 
class SimulationWindow
	{
	public:
		SimulationWindow ( int x, int y, int w, int h);
		~SimulationWindow();

		// shows char #c at #x #y coordinates with #colour_pair.
		void print ( char c, int x, int y, int colour_pair);


		// Sets text that will appear at the bottom of the window.
		void clear();
		void refresh();
		int get_height () const;
		int get_width () const;
		int get_x () const;
		int get_y () const;

	private:

		/// ncurses internal data structure.
		WINDOW * wind;

		int x;
		int y;
		int width;
		int height;

	};
	
	
#endif //CWINDOW_H_DEFINED