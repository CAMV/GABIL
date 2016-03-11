cmake_minimum_required(VERSION 3.2)
project(GABIL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    GA.cpp
    GabilInd.cpp)

add_executable(GABIL ${SOURCE_FILES})