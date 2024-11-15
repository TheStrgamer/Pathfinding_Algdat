cmake_minimum_required(VERSION 3.29)
project(oving7_pathfinding)

set(CMAKE_CXX_STANDARD 14)

add_executable(oving7_pathfinding
    oving7.cpp
        loadData.hpp
        dijkstra.hpp
        precompute.hpp
        ALT.hpp)
