cmake_minimum_required(VERSION 3.15)
project(AntNest)

#set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)



add_executable(AntNest main.cpp AntNest.hpp AntNest.cpp Ant.hpp Ant.cpp)

target_link_directories(AntNest PUBLIC
        "$(PROJECT_BINARY_DIR)"
        )
target_link_libraries(AntNest PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        ${CURSES_LIBRARIES}
        )
