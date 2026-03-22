cmake_minimum_required(VERSION 3.14)
project(vector_math LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(vector_math vector3d.cpp)