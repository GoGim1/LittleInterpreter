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
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories(src)
include_directories(src/Ast)

if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        add_subdirectory(src)
endif()

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        add_subdirectory(tests)
endif()

