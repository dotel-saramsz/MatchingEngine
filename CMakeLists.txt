cmake_minimum_required(VERSION 3.13)
project(MatchingEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(MatchingEngine main.cpp orderparser.cpp orderparser.h)