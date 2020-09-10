#include <random>
#include <cmath>
#include <iomanip>
#include <utility>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <condition_variable>

#include "Ant.hpp"
#include "AntNest.hpp"
#include "ncursesEngine/NC2DArray.hpp"
#include "ncursesEngine/SimulationWindow.hpp"


extern std::mutex ant_mutex;
extern std::mutex insect_mutex;
extern std::mutex entrance_mutex;
extern NC2DArray board;
extern std::atomic_bool end_flag, empty_entrance;


std::condition_variable entrance_cv;


// ant with defined life time and random job
Ant::Ant(const int lt_, AntNest &nest_)
        : life_time(lt_), nest(nest_) {
    // tool for randomness
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distr(0, 2);

    // random job for ant 0 - food supplier, 1 - warrior, 2 - babysitter
    job = static_cast<int>(std::round(distr(g)));
    ant_coord = nest.get_nest_coord();
    run();

}

// getters
int &Ant::get_life_time() { return life_time; }

int &Ant::get_job() { return job; }

std::thread &Ant::get_thread() { return ant_thread; }

void Ant::run() {
    ant_thread = std::thread{[this]() { this->work(); }};
}


// depends of job - ant do something until dead
void Ant::work() {
    while (life_time > 0 && !end_flag) {
        switch (job) {
            case 0:
                find_food();
                break;

            case 1:
                attack_insect();
                break;

            case 2:
                babysit_eggs();
                break;

            default:
                break;
        }
        life_time -= 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Ant::find_food() {

    auto random_in_range = [](int min, int max) {

        static std::mt19937 mt(std::random_device{}());
        static std::uniform_int_distribution<int> pick;

        return pick(mt, decltype(pick)::param_type{min, max});
    };

    int food_location = random_in_range(0, nest.get_food_source().size());

    int x = random_in_range(0, board.height() - 1),
            y = random_in_range(0, board.width() - 1);
    std::pair<int, int> food_coord(x, y);


    if (board.at(ant_coord.first, ant_coord.second) != 'O') {
        board.set(food_coord.first, food_coord.second, '.');
    }

    std::unique_lock<std::mutex> entrance_lock(entrance_mutex);
    entrance_cv.wait(entrance_lock, []() { return &empty_entrance; });
    empty_entrance = false;
    {
        board.set(ant_coord.first, ant_coord.second, '^');
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    empty_entrance = true;
    entrance_lock.unlock();
    entrance_cv.notify_one();


    while (food_coord != ant_coord) {
        if (board.at(ant_coord.first, ant_coord.second) != 'O' and ant_coord != nest.get_nest_coord()) {
            board.set(ant_coord.first, ant_coord.second, ' ');
        }

        food_coord.first > ant_coord.first ? ant_coord.first++ : food_coord.first == ant_coord.first
                                                                 ?: ant_coord.first--;
        food_coord.second > ant_coord.second ? ant_coord.second++ : food_coord.second == ant_coord.second
                                                                    ?: ant_coord.second--;

        if (board.at(ant_coord.first, ant_coord.second) != 'O' and ant_coord != nest.get_nest_coord()) {
            board.set(ant_coord.first, ant_coord.second, '-');
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    std::unique_lock<std::mutex> food_lock(ant_mutex);
    if (nest.get_food_source()[food_location] > 0) {
        nest.increment_food(1);
    }
    food_lock.unlock();

    while (nest.get_nest_coord() != ant_coord) {
        if (board.at(ant_coord.first, ant_coord.second) != 'O' and ant_coord != nest.get_nest_coord()) {
            board.set(ant_coord.first, ant_coord.second, ' ');
        }

        nest.get_nest_coord().first > ant_coord.first ? ant_coord.first++ : nest.get_nest_coord().first ==
                                                                            ant_coord.first ?: ant_coord.first--;
        nest.get_nest_coord().second > ant_coord.second ? ant_coord.second++ : nest.get_nest_coord().second ==
                                                                               ant_coord.second ?: ant_coord.second--;

        if (board.at(ant_coord.first, ant_coord.second) != 'O' and ant_coord != nest.get_nest_coord()) {
            board.set(ant_coord.first, ant_coord.second, '-');
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    entrance_lock.lock();
    entrance_cv.wait(entrance_lock, []() { return &empty_entrance; });
    empty_entrance = false;
    {
        board.set(ant_coord.first, ant_coord.second, '^');
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    empty_entrance = true;
    entrance_lock.unlock();
    entrance_cv.notify_one();


}

void Ant::babysit_eggs() {

}

void Ant::attack_insect() {

    while (nest.get_environment().get_insect_coord() != ant_coord) {
        if (board.at(ant_coord.first, ant_coord.second) != 'O') {
            board.set(ant_coord.first, ant_coord.second, ' ');
        }

        nest.get_environment().get_insect_coord().first > ant_coord.first
        ? ant_coord.first++ : nest.get_environment().get_insect_coord().first == ant_coord.first ?: ant_coord.first--;
        nest.get_environment().get_insect_coord().second > ant_coord.second
        ? ant_coord.second++ : nest.get_environment().get_insect_coord().second == ant_coord.second
                               ?: ant_coord.second--;


        if (board.at(ant_coord.first, ant_coord.second) != 'O') {
            board.set(ant_coord.first, ant_coord.second, 'o');
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::unique_lock<std::mutex> lock(insect_mutex);
    if (--nest.get_insect() == 0) {
        nest.increment_food(50);
        board.set(ant_coord.first, ant_coord.second, nest.get_insect());
    } else {
        if (nest.get_insect() < 0) {
            nest.get_insect() = -1;
            board.set(ant_coord.first, ant_coord.second, ' ');
        }
    }
    lock.unlock();

    while (nest.get_nest_coord() != ant_coord) {
        if (board.at(ant_coord.first, ant_coord.second) != 'O') {
            board.set(ant_coord.first, ant_coord.second, ' ');
        }

        nest.get_nest_coord().first > ant_coord.first ? ant_coord.first++ : nest.get_nest_coord().first ==
                                                                            ant_coord.first ?: ant_coord.first--;
        nest.get_nest_coord().second > ant_coord.second ? ant_coord.second++ : nest.get_nest_coord().second ==
                                                                               ant_coord.second ?: ant_coord.second--;

        if (board.at(ant_coord.first, ant_coord.second) != 'O') {
            board.set(ant_coord.first, ant_coord.second, 'o');
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}