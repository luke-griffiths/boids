cmake_minimum_required(VERSION 3.6)
project(boids)
add_subdirectory(pybind11)
pybind11_add_module(pybindings pybindings.cpp boids.cpp)
