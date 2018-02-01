cmake_minimum_required(VERSION 3.8)
project(GA_Reversi)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Board.cpp Board.h Game.cpp Game.h Triple.cpp Triple.h Heuristic.cpp Heuristic.h)
add_executable(GA_Reversi_Threaded ${SOURCE_FILES})

target_link_libraries(GA_Reversi_Threaded libSDL2main.a libSDL2.dll.a)