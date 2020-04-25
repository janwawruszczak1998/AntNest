#ifndef ENVIRONMENT_INCLUDE
#define ENVIRONMENT_INCLUDE


#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <cmath>
#include <algorithm>


static std::atomic_bool end_flag;

class Environment{
public:
    Environment();    
    Environment(const int);

    ~Environment();

    void run();
    void environmetn_activity();

    std::atomic<int>& get_insect();
    std::vector<int>& get_food_sources();
    std::thread& get_environment_thread();

private:
    std::vector<int> food_sources;
    std::atomic<int> insect;
    std::thread environment_thread;

};


#endif