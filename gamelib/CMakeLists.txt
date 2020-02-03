cmake_minimum_required(VERSION 3.13)
project(gamelib)
include_directories(${PROJECT_SOURCE_DIR})

add_library(gamelib STATIC
    gamelib.cpp
	gamelib_actor.cpp
	gamelib_audio.cpp
	gamelib_command.cpp
	gamelib_component.cpp
    gamelib_context.cpp
	gamelib_graphics.cpp
	gamelib_input_handler.cpp
	gamelib_locator.cpp
    gamelib_object.cpp
	gamelib_random.cpp
    gamelib_world.cpp
    hatchetfish_log.cpp
    hatchetfish_stopwatch.cpp
    )

if (!APPLE)
target_precompile_headers(gamelib PRIVATE pch.h)
endif()

install(TARGETS gamelib DESTINATION lib)
#[[install(TARGETS
    ./gamelib.hpp
    ./gamelib_base.hpp
    ./gamelib_object.hpp
    ./gamelib_actor.hpp
    ./gamelib_world.hpp
    DESTINATION include)
]]
