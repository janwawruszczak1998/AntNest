#include <iostream>
#include <vector>
#include <ncurses.h>
#include "AntNest.hpp"
#include "Ant.hpp"

int main(){


//auto ant_nest = std::make_shared<AntNest>();
    //ant_nest->get_nest_thread().join();
  
    AntNest ant_nest;
    ant_nest.get_nest_thread().join();



    return 0;
}