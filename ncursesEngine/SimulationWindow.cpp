#include "SimulationWindow.hpp"

#include <sstream>                // stringstream
#include <iostream>

#define FILL_WINDOW 0

SimulationWindow::SimulationWindow(int x, int y, int w, int h)
        : wind(nullptr), x(x), y(y), width(w), height(h) {

    wind = newwin(height, width, y, x);
    if (!wind) {
        throw "popsute ncurses";
    }

}

SimulationWindow::~SimulationWindow() {
    if (wind) {
        delwin(wind);
    }
}


void SimulationWindow::print(char c, int x, int y, int colour_pair) {

    wattron  (wind, A_BOLD);
    wattron  (wind, colour_pair);
    mvwaddch (wind, y, x, c);
    wattroff (wind, colour_pair);
    wattroff  (wind, A_BOLD);

}

void SimulationWindow::clear() {
    werase(wind);
}

void SimulationWindow::refresh() {
    wrefresh(wind);
}

int SimulationWindow::get_height() const {
    return height;
}

int SimulationWindow::get_width() const {
    return width;
}

int SimulationWindow::get_x() const {
    return x;
}

int SimulationWindow::get_y() const {
    return y;
}

