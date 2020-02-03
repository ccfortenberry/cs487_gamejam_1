cmake_minimum_required(VERSION 3.13)
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()
if(LINUX)
    set(CMAKE_CXX_COMPILER "/bin/g++-9")
endif()
project(cs487game)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (APPLE)
    include_directories(/usr/local/include)
    set(STD_CXX_FS c++fs)
endif()

add_subdirectory(gamelib)
add_subdirectory(simplegame)

