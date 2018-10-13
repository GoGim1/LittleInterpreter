cmake_minimum_required(VERSION 3.3)
project(LittleInterpreter)

SET(CMAKE_BUILD_TYPE "Debug")

set(CXX_FLAGS
        -Wall
        -Wold-style-cast #warn if C-style implicit-casting occurs
        -Wconversion
        )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g2 -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(src)

#add_subdirectory(src)
add_subdirectory(tests)

