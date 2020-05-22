#include <iostream>
#include <vector>
#include <deque>
#include <ncurses.h>
#include <mutex>
#include <atomic>
#include "AntNest.hpp"
#include "Ant.hpp"
#include "Environment.hpp"
#include "ncursesEngine/NCWrapper.hpp"
#include "ncursesEngine/SimulationWindow.hpp"
#include "ncursesEngine/NC2DArray.hpp"

std::atomic_bool end_flag;
std::mutex ant_mutex;
std::mutex insect_mutex;
std::mutex food_mutex;

NC2DArray board(25, 100);

void draw(SimulationWindow* window) {
    while(!end_flag){
        for(int i = 0; i < board.height(); ++i){
            for(int j = 0; j < board.width(); ++j){
                window->print(static_cast<int>(board.at(i, j)), i, j, COLOR_PAIR(PAIR_GREEN_BLACK));
            }
        }
        window->refresh();
    }
}

int main(){
    

    std::cout << "Poczekaj, az pozostale watki sie zakoncza" << std::endl;
    Environment environment(10);
    AntNest ant_nest1(1, environment);
    AntNest ant_nest2(2, environment);

    NCWRAPPER::init();
    SimulationWindow* sim_window = new SimulationWindow(0, 0, 200, 200);
    std::thread draw_thread = std::thread{ [sim_window](){ draw(sim_window); } };

    while (true){
        if (std::getchar() == 'q')
        {
            end_flag = true;
            break;
        }
    }

    
    draw_thread.join();
    NCWRAPPER::exit();

    ant_nest1.get_nest_thread().join();
    ant_nest2.get_nest_thread().join();
    environment.get_environment_thread().join();

    
    return 0;
}