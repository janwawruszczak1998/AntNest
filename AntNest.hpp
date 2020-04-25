#ifndef ANT_NEST_INCLUDE
#define ANT_NEST_INCLUDE



#include <vector>
#include <memory>
#include <thread>
#include <algorithm>

#include "Environment.hpp"
#include "Ant.hpp"

class Ant;
class Environment;

class AntNest{
public:
    AntNest(Environment& environment);
    ~AntNest();


    void produce_ant(int life_time);
    void increment_food(int);

    void produce_egg();
    void increment_egg();
    
    void run();
    void nest_activities();

    std::thread& get_nest_thread();
    std::vector<std::unique_ptr<Ant>>& get_ants();
    std::vector<int>& get_food_source();
    std::atomic<int>& get_insect();

private:
    int food;
    int eggs;
    std::vector<std::unique_ptr<Ant> > ants;
    std::thread nest_life;
    Environment& nest_environment;

    int nest_size_limit;
};

#endif