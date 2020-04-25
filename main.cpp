#include <iostream>
#include <vector>
#include <ncurses.h>
#include "AntNest.hpp"
#include "Ant.hpp"
#include "Environment.hpp"

std::atomic_bool end_flag(false);

int main(){

    std::cout << "end_flag" << end_flag << std::endl;

    Environment environment(10);
    AntNest ant_nest(environment);

    char input_sign;
    while (true){
        if (std::getchar() == 'q')
        {
            end_flag = true;
            break;
        }
    }
    std::cout << "!ef!" << end_flag << std::endl;
    environment.get_environment_thread().join();
    ant_nest.get_nest_thread().join();



    return 0;
}