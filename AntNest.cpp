#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <iterator>

#include "Environment.hpp"
#include "AntNest.hpp"
#include "Ant.hpp"
#include "ncursesEngine/NC2DArray.hpp"


const int LIFE_TIME = 1000000;
const int NEST_SIZE = 100;

extern std::atomic_bool end_flag;
extern NC2DArray board;

AntNest::AntNest(int id, Environment& environment) 
: nest_id(id), nest_environment(environment), food(3), eggs(3), nest_size_limit(NEST_SIZE) {

    if(id == 1){
        nest_coord = std::make_pair(3,3);
    }
    else{
        nest_coord = std::make_pair(board.height()-4, board.width()-1);
    }

    for(int i = 0; i < nest_size_limit; ++i){
        ants.push_back(std::make_unique<Ant>(LIFE_TIME, *this));
    }

    

    this->run();
}

AntNest::~AntNest(){
}

std::thread& AntNest::get_nest_thread(){ return nest_life; }
std::vector<std::unique_ptr<Ant>>& AntNest::get_ants(){ return ants; }
std::vector<int>& AntNest::get_food_source(){ return nest_environment.get_food_sources(); }
std::atomic<int>& AntNest::get_insect(){ return nest_environment.get_insect(); } 
Environment& AntNest::get_environment(){ return nest_environment; }
int AntNest::get_id(){ return nest_id; }
std::pair<int, int> AntNest::get_nest_coord(){ return nest_coord; }


void AntNest::produce_ant(int ant_life_time){
    if(eggs > 0){
        --eggs;
        std::vector<std::unique_ptr<Ant> >::iterator dead_ant = 
            std::find_if(ants.begin(), ants.end(), 
                [](std::unique_ptr<Ant>& ant){ return ant->get_life_time() <= 0; });

        if(dead_ant != ants.end()){
            dead_ant->get()->get_thread().join();
            dead_ant->get()->get_life_time() = LIFE_TIME;
            dead_ant->get()->run();
        }
        
    }
}

void AntNest::produce_egg(){
    if( !food > 0 ) {
        return;
    }

    // if we have baby_sitter that is alive we are not dead yet
    if(std::any_of(ants.begin(), ants.end(), [](std::unique_ptr<Ant>& ant){ return ant->get_life_time() > 0; } )
        && std::any_of(ants.begin(), ants.end(), [](std::unique_ptr<Ant>& ant){ return ant->get_job() == 2; } ) ) {
        food--;
        eggs++;
    }
   
}

void AntNest::run(){
    nest_life = std::thread{ [this](){this->nest_activities(); } };
}



void AntNest::nest_activities(){
    // tool for randomness
    std::random_device rd;
    std::mt19937 g(rd());
    std::normal_distribution<> normal_dist(1, 2);

    // randomly nest produces eggs and ants(using eggs), until we have some resources
    while( (food > 0 or eggs > 0)  && !end_flag){
        

        if( static_cast<int>(std::round(normal_dist(g))) % 2 == 0){
            produce_ant(LIFE_TIME); 
        }
        else{
            produce_egg();
        }
    }

    
    auto kill_nest = [](std::unique_ptr<Ant>& ant_to_kill){
            if(ant_to_kill->get_thread().joinable()){
                ant_to_kill->get_thread().join();
            } 
        };

    std::for_each(ants.begin(), ants.end(), kill_nest);
        
}



void AntNest::increment_food(int food_delivered){
    food += food_delivered;
}

void AntNest::increment_egg(){
    eggs++;
}

