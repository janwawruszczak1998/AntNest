#ifndef ENVIRONMENT_INCLUDE
#define ENVIRONMENT_INCLUDE


#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <cmath>
#include <algorithm>


class Environment {
public:
    Environment(const int);

    ~Environment();

    void run();

    void environment_activity();

    std::atomic<int> &get_insect();

    std::vector<int> &get_food_sources();

    std::thread &get_environment_thread();

    std::pair<int, int> get_insect_coord();

private:
    std::vector<int> food_sources;
    std::atomic<int> insect;
    std::thread environment_thread;
    std::pair<int, int> insect_coord;

};


#endif