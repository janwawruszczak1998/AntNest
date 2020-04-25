#include <iostream>
#include <vector>
#include <ncurses.h>
#include "AntNest.hpp"
#include "Ant.hpp"
#include "Environment.hpp"

int main(){

    end_flag = false;

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

    environment.get_environment_thread().join();
    ant_nest.get_nest_thread().join();



    return 0;
}