#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>
#include <mutex>
#include <atomic>
#include "AntNest.hpp"
#include "Ant.hpp"
#include "Environment.hpp"
#include "ncursesEngine/NCWrapper.hpp"
#include "ncursesEngine/SimulationWindow.hpp"
#include "ncursesEngine/NC2DArray.hpp"

std::atomic_bool end_flag, empty_entrance;
std::mutex ant_mutex, insect_mutex, food_mutex, entrance_mutex;



const int WINDOW_X = 0, WINDOW_Y = 0, WINDOW_HEIGHT = 25, WINDOW_WIDTH = 100,
            BOARD_WIDTH = 25, BOARD_HEIGHT = 100;

NC2DArray board(BOARD_WIDTH, BOARD_HEIGHT);

void draw_info(SimulationWindow*, std::string);
void draw_nests();

void draw_window(SimulationWindow* window) {
    while(!end_flag){
        for(int i = 0; i < board.height(); ++i){
            for(int j = 0; j < board.width(); ++j){
                if(board.at(i, j) == '.'){
                    window->print(static_cast<int>(board.at(i, j)), i, j, COLOR_PAIR(PAIR_YELLOW_BLACK));
                }
                else{
                    if(board.at(i, j) == '-'){
                        window->print(static_cast<int>(board.at(i, j)), i, j, COLOR_PAIR(PAIR_RED_BLACK));
                    }
                    else{
                        if(board.at(i, j) == '~'){
                            window->print(static_cast<int>(board.at(i, j)), i, j, COLOR_PAIR(PAIR_RED_BLACK));
                        }
                        else{
                            window->print(static_cast<int>(board.at(i, j)), i, j, COLOR_PAIR(PAIR_GREEN_BLACK));
                        }
                    }
                }
                
            }
        }
        draw_nests();
        window->refresh();
    }
}


int main(){
    

    std::cout << "Poczekaj, az pozostale watki sie zakoncza" << std::endl;

    Environment environment(10);
    AntNest ant_nest1(1, environment);
    AntNest ant_nest2(2, environment);
    draw_nests();

    NCWRAPPER::init();
    SimulationWindow* info_window = new SimulationWindow(5, 2, 30, 5);
    SimulationWindow* sim_window = new SimulationWindow(WINDOW_X, WINDOW_Y + 5, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    draw_info(info_window, "q - aby zakonczyc");

    
    std::thread draw_thread = std::thread{ [sim_window](){  draw_window(sim_window); } };

    while (!end_flag){
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


void draw_nests(){
    const int back_slash = 92;
    // first nest
    board.set(1, 2, '/');
    board.set(2, 1, '/');
    board.set(3, 0, '_');
    board.set(4, 1, back_slash);
    board.set(5, 2, back_slash);
    
    
    // second nest
    board.set(BOARD_HEIGHT - 6, BOARD_WIDTH -2, '/');
    board.set(BOARD_HEIGHT - 5, BOARD_WIDTH -3, '/');
    board.set(BOARD_HEIGHT - 4, BOARD_WIDTH -4, '_');
    board.set(BOARD_HEIGHT - 3, BOARD_WIDTH -3, back_slash);
    board.set(BOARD_HEIGHT - 2, BOARD_WIDTH -2, back_slash);
   
}

void draw_info(SimulationWindow* info_wind, std::string info){
    
    for(int i = 0; i < info.size(); ++i){
        info_wind->print(static_cast<int>(info[i]), i + 2, 2, COLOR_PAIR(PAIR_WHITE_BLACK));
    }
    info_wind->refresh();
    
}