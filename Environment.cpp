#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include "Environment.hpp"


extern std::atomic_bool end_flag;

// default environment has no food sources and is dead at all
Environment::Environment()
: food_sources(0), insect(0){

}

// defined number of food sources
Environment::Environment(const int num_of_food_sources)
: food_sources(num_of_food_sources, 0), insect(15){
    run();
}


Environment::~Environment(){
    food_sources.clear();
}

std::atomic<int>& Environment::get_insect(){ return insect; }
std::vector<int>& Environment::get_food_sources(){ return food_sources; }
std::thread& Environment::get_environment_thread(){ return environment_thread; }

void Environment::run(){
    environment_thread = std::thread{ [this](){ this->environmetn_activity(); } };
}

void Environment::environmetn_activity(){

    while(!end_flag){

        std::cout << "insects life status: " << insect << std::endl;
        for(int i = 0; i < food_sources.size(); ++i){
            std::cout << "[" <<i << ": " << food_sources[i] << "]";
        }
        std::cout << std::endl;


        auto add_food = [](int& actual_food){
            // tool for randomness
            std::random_device rd;
            std::mt19937 g(rd());
            std::uniform_int_distribution<> distr(1, 5);    // adding 1-5 food

            int additional_food = static_cast<int>(std::round(distr(g)));
            actual_food = actual_food + additional_food ? actual_food + additional_food : 1e9;
        };

        std::for_each(food_sources.begin(), food_sources.end(), add_food);
        insect = 15;

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    
}
