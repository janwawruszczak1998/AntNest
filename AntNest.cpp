#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <iterator>

#include "AntNest.hpp"
#include "Ant.hpp"

AntNest::AntNest() : food(3), eggs(3), nest_size_limit(15) {
    std::cout << "Nest created" << std::endl;
    
    for(int i = 0; i < nest_size_limit; ++i){
        ants.push_back(std::make_unique<Ant>(1000000, *this));
    }

    this->run();
}

AntNest::~AntNest(){
    ants.clear();
}

std::thread& AntNest::get_nest_thread(){ return nest_life; }

void AntNest::produce_ant(int ant_life_time){
    std::cout << "make ant" << std::endl;

    if(eggs > 0){
        --eggs;
        std::vector<std::unique_ptr<Ant> >::iterator dead_ant = 
            std::find_if(ants.begin(), ants.end(), 
                [](std::unique_ptr<Ant>& ant){ return ant->get_life_time() <= 0; });

        if(dead_ant != ants.end()){
            dead_ant->get()->get_thread().join();
            dead_ant->get()->get_life_time() = 1000000;
            dead_ant->get()->run();
        }
        
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
}

void AntNest::produce_egg(){
    std::cout << "produce egg" << std::endl;
    if( !food > 0 ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(900)); 
        return;
    }

    food--;

    // if we have baby_sitter that is alive we are not dead yet
    if(std::any_of(ants.begin(), ants.end(), [](std::unique_ptr<Ant>& ant){ return ant->get_life_time() > 0; } )
        && std::any_of(ants.begin(), ants.end(), [](std::unique_ptr<Ant>& ant){ return ant->get_job() == 2; } ) ) {
        
        eggs++;
    }
   
    std::this_thread::sleep_for(std::chrono::milliseconds(900)); 
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
    while(food > 0 or eggs > 0 ){
        std::cout << "food: " << food << " eggs: " << eggs << std::endl;
        

        if( static_cast<int>(std::round(normal_dist(g))) % 2 == 0){
            produce_ant(1000000); 
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