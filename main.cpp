#include <iostream>
#include <vector>
#include <deque>
#include <ncurses.h>
#include <mutex>
#include <atomic>
#include "AntNest.hpp"
#include "Ant.hpp"
#include "Environment.hpp"

std::atomic_bool end_flag;
std::mutex ant_mutex;
std::mutex insect_mutex;
std::mutex food_mutex;


int main(){

    Environment environment(10);
    AntNest ant_nest1(1, environment);
    AntNest ant_nest2(2, environment);

    char input_sign;
    while (true){
        if (std::getchar() == 'q')
        {
            end_flag = true;
            break;
        }
    }

    environment.get_environment_thread().join();
    ant_nest1.get_nest_thread().join();
    ant_nest1.get_nest_thread().join();



    return 0;
}