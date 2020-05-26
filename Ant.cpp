#include <random>
#include <cmath>
#include <iomanip>
#include <utility>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>

#include "Ant.hpp"
#include "AntNest.hpp"
#include "ncursesEngine/NC2DArray.hpp"

extern std::mutex ant_mutex;
extern std::mutex insect_mutex;
extern NC2DArray board;
extern std::atomic_bool end_flag;



// ant with defined life time and random job
Ant::Ant(const int lt_, AntNest& nest_) : life_time(lt_), nest(nest_)
{
    // tool for randomness
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distr(0, 2);

    // random job for ant 0 - foodbarer, 1 - warior, 2 - babysitter
    job = static_cast<int>(std::round(distr(g)));
    ant_coord = nest.get_nest_coord();
    run();

}

// ant with defined life time and job
Ant::Ant(const int lt_, const int job_, AntNest& nest_) : life_time(lt_), nest(nest_)
{
    ant_coord = nest.get_nest_coord();
    run();
}



Ant::~Ant(){
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
    while(life_time > 0 && !end_flag){
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
        life_time -= 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Ant::find_food(){
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> location_distr(0, nest.get_food_source().size() - 1);
    std::uniform_int_distribution<> coord_distr(0, std::max(board.width(), board.height()));

    int food_location = static_cast<int>(std::round(location_distr(g)));

    int x = static_cast<int>(std::round(coord_distr(g))) % board.height(), 
        y = static_cast<int>(std::round(coord_distr(g))) % board.width();
    std::pair<int, int> food_coord(x,y);

    if(board.at(ant_coord.first, ant_coord.second) != 'O'){
        board.set(food_coord.first, food_coord.second, '.');
    }
    

    
    while(food_coord != ant_coord){
        if(board.at(ant_coord.first, ant_coord.second) != 'O'){
            board.set(ant_coord.first, ant_coord.second, ' ');
        }
        
        food_coord.first > ant_coord.first ? ant_coord.first++ : food_coord.first == ant_coord.first ? : ant_coord.first--;
        food_coord.second > ant_coord.second ? ant_coord.second++ : food_coord.second == ant_coord.second ? : ant_coord.second--;

        if(board.at(ant_coord.first, ant_coord.second) != 'O'){
            board.set(ant_coord.first, ant_coord.second, '-');
        }
        

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    
    std::unique_lock<std::mutex> lock(ant_mutex);
    if(nest.get_food_source()[food_location] > 0){
        nest.increment_food(1);
    }
    lock.unlock();
    
    while(nest.get_nest_coord() != ant_coord){
        board.set(ant_coord.first, ant_coord.second, ' ');
        nest.get_nest_coord().first > ant_coord.first ? ant_coord.first++ : nest.get_nest_coord().first == ant_coord.first ? : ant_coord.first--;
        nest.get_nest_coord().second > ant_coord.second ? ant_coord.second++ : nest.get_nest_coord().second == ant_coord.second ? : ant_coord.second--;
        board.set(ant_coord.first, ant_coord.second, '-');
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

}

void Ant::babysit_eggs(){

}

void Ant::attack_insect(){

    while( nest.get_environment().get_insect_coord() != ant_coord){
        if(board.at(ant_coord.first, ant_coord.second) != 'O'){
            board.set(ant_coord.first, ant_coord.second, ' ');
        }

        nest.get_environment().get_insect_coord().first > ant_coord.first 
            ? ant_coord.first++ : nest.get_environment().get_insect_coord().first == ant_coord.first ? : ant_coord.first--;
        nest.get_environment().get_insect_coord().second > ant_coord.second 
            ? ant_coord.second++ : nest.get_environment().get_insect_coord().second == ant_coord.second ? : ant_coord.second--;

        
        if(board.at(ant_coord.first, ant_coord.second) != 'O'){
            board.set(ant_coord.first, ant_coord.second, 'o');
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::unique_lock<std::mutex> lock(insect_mutex);
    if(--nest.get_insect() == 0){
        nest.increment_food(50);
        board.set(ant_coord.first, ant_coord.second, nest.get_insect());
    }
    else{
        if(nest.get_insect() < 0){
            nest.get_insect() = -1;
            board.set(ant_coord.first, ant_coord.second, ' ');
        } 
    }
    lock.unlock();

    while(nest.get_nest_coord() != ant_coord){
        if(board.at(ant_coord.first, ant_coord.second) != 'O'){
            board.set(ant_coord.first, ant_coord.second, ' ');
        }

        nest.get_nest_coord().first > ant_coord.first ? ant_coord.first++ : nest.get_nest_coord().first == ant_coord.first ? : ant_coord.first--;
        nest.get_nest_coord().second > ant_coord.second ? ant_coord.second++ : nest.get_nest_coord().second == ant_coord.second ? : ant_coord.second--;

        if(board.at(ant_coord.first, ant_coord.second) != 'O'){
            board.set(ant_coord.first, ant_coord.second, 'o');
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}