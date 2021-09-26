cmake_minimum_required(VERSION 3.20)

set(PRJ oslab1)

project(${PRJ})

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        src/main.cpp)
set(INCLUDE
        src/Synchronized.h
        src/Calculator.h)

add_executable(${PRJ} ${INCLUDE} ${SOURCES})
