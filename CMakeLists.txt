cmake_minimum_required(VERSION 3.5)
project(final-project-compilers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp finalv1.txt finalv2.txt myHeader.h cleanFile.h PredictiveParser.cpp PredictiveParser.h)
add_executable(Compiler ${SOURCE_FILES})