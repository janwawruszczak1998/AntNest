#include <random>
#include <cmath>
#include <iomanip>
#include <utility>
#include <iostream>
#include <thread>
#include <chrono>

#include "Ant.hpp"
#include "AntNest.hpp"

// ant with defined life time and random job
Ant::Ant(const int lt_, AntNest& nest_) : life_time(lt_), nest(nest_)
{
    // tool for randomness
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distr(0, 2);

    // random job for ant 0 - foodbarer, 1 - warior, 2 - babysitter
    job = static_cast<int>(std::round(distr(g)));
    run();
}

// ant with defined life time and job
Ant::Ant(const int lt_, const int job_, AntNest& nest_) : life_time(lt_), nest(nest_)
{
    run();
}



Ant::~Ant(){
    //nest.reset();
}

// getters
int& Ant::get_life_time(){ return life_time; }
int& Ant::get_job(){ return job; }
std::thread& Ant::get_thread(){ return ant_thread; }

void Ant::run(){
    ant_thread = std::thread{ [this](){ this->work(); } };
}


// depends of job - ant do something until dead
void Ant::work(){
    while(life_time > 0){
        if(job == 0){
            find_food();
        }
        else{
            if(job == 1){
                attack_insect();
            }
            else{
                babysit_eggs();
            }
        }
        life_time -= 100000;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Ant::find_food(){
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distr(0, nest.get_food_source().size() - 1);

    int food_location = static_cast<int>(std::round(distr(g)));
    if(nest.get_food_source()[food_location] > 0){
        nest.increment_food(1);
    }


}

void Ant::babysit_eggs(){

}

void Ant::attack_insect(){
    if(--nest.get_insect() == 0){
        nest.increment_food(20);
    }
    else{
        if(nest.get_insect() < 0) nest.get_insect() = -1;
    }
}