#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <utility>
#include <random>

#include "Environment.hpp"
#include "ncursesEngine/NC2DArray.hpp"


extern std::atomic_bool end_flag;
extern std::mutex food_mutex;
extern std::mutex insect_mutex;
extern NC2DArray board;

constexpr int INSECT_LIFE = 15;

// defined number of food sources
Environment::Environment(const int num_of_food_sources)
: food_sources(num_of_food_sources, 0), insect(INSECT_LIFE) {
    insect_coord = std::make_pair(0, 0);
    run();
}


Environment::~Environment() {
    food_sources.clear();
}

std::atomic<int> &Environment::get_insect() { return insect; }

std::vector<int> &Environment::get_food_sources() { return food_sources; }

std::thread &Environment::get_environment_thread() { return environment_thread; }

std::pair<int, int> Environment::get_insect_coord() { return insect_coord; }

void Environment::run() {
    environment_thread = std::thread{[this]() { this->environment_activity(); }};
}

void Environment::environment_activity() {

    while (!end_flag) {

        // tool for randomness
        std::random_device rd;
        std::mt19937 g(rd());

        auto add_food = [](int &actual_food) {
            std::uniform_int_distribution<> food_distr(1, 5);    // adding 1-5 food
            std::random_device rd;
            std::mt19937 g(rd());
            int additional_food = static_cast<int>(std::round(food_distr(g)));
            const std::lock_guard<std::mutex> lock(food_mutex);
            actual_food = actual_food + additional_food ? actual_food + additional_food : 1e9;
        };

        std::for_each(food_sources.begin(), food_sources.end(), add_food);


        if (insect <= 0) {
            board.set(insect_coord.first, insect_coord.second, ' ');

            std::uniform_int_distribution<> insect_distr(0, std::max(board.width(), board.height()));
            int x = static_cast<int>(std::round(insect_distr(g))) % board.height(),
                y = static_cast<int>(std::round(insect_distr(g))) % board.width();
            insect_coord = std::make_pair(x, y);

            board.set(insect_coord.first, insect_coord.second, 'O');
            insect = INSECT_LIFE;
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

}
