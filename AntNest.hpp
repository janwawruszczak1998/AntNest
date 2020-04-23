#ifndef ANT_NEST_INCLUDE
#define ANT_NEST_INCLUDE



#include <vector>
#include <memory>
#include <thread>

#include "Ant.hpp"

class Ant;

class AntNest{
public:
    AntNest();
    ~AntNest();

    // nest activities
    void produce_ant(int life_time);
    void increment_food(int);

    void produce_egg();
    void increment_egg();
    
    void run();
    void nest_activities();

    std::thread& get_nest_thread();

private:
    int food;
    int eggs;
    std::vector<std::unique_ptr<Ant> > ants;
    std::thread nest_life;

    int nest_size_limit;
};

#endif