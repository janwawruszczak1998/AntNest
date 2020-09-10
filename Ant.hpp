#ifndef ANT_INCLUDE
#define ANT_INCLUDE

#include <thread>
#include "AntNest.hpp"
#include <random>

class AntNest;


class Ant {
public:
    Ant(const int, AntNest &);       // lifetime as atribute, random job

    void work();

    void run();

    void find_food();

    void attack_insect();

    void babysit_eggs();

    // getters/setters
    int &get_life_time();

    int &get_job();

    std::thread &get_thread();

private:
    int life_time;
    int job;
    std::thread ant_thread;
    AntNest &nest;
    std::pair<int, int> ant_coord;

};

#endif