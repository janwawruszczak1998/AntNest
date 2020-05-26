#ifndef ANT_INCLUDE
#define ANT_INCLUDE

#include <thread>
#include "AntNest.hpp"

class AntNest;

class Ant{
public:
    Ant();          // default ant is dead worker without nest
    Ant(const int,AntNest&);       // lifetime as atribute, random job
    Ant(const int, const int, AntNest&);  // lifetime and job as atribute


    ~Ant();

    // ant activities
    void work();
    void run();

    void find_food();
    void attack_insect();
    void babysit_eggs();

    // getters/setters
    int& get_life_time();
    int& get_job();
    std::thread& get_thread();

private:
    int life_time;
    int job;
    std::thread ant_thread;
    AntNest& nest;
    std::pair<int, int> ant_coord;
};

#endif