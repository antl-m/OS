cmake_minimum_required(VERSION 3.20)

set(PRJ oslab1)

project(${PRJ})

set(CMAKE_CXX_STANDARD 20)

set(SOURCES)
set(INCLUDE
        src/Synchronized.h
        src/Calculator.h
        src/ConditionVariable.h)
set(TEST_SRC
        src/test/trialfuncs.hpp
        src/test/compfuncs.hpp
        src/test/test.cpp)
set(INCLUDE_DIRS
        src
        src/test)

add_executable(${PRJ} ${INCLUDE} ${SOURCES} src/main.cpp)
add_executable(${PRJ}_test ${INCLUDE} ${SOURCES} ${TEST_SRC})

target_include_directories(${PRJ} PUBLIC ${INCLUDE_DIRS})
target_include_directories(${PRJ}_test PUBLIC ${INCLUDE_DIRS})
