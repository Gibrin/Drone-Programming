cmake_minimum_required(VERSION 3.14)
project(Vector3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    main.cpp
    Vector3D.cpp
    Vector3D.h
)