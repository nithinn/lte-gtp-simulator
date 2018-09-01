# GTP Simulator cmake file
cmake_minimum_required(VERSION 2.8.9)
project(gsim)
 
find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral \
    -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef \
    -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter")

include_directories(src ${CURSES_INCLUDE_DIR})

file(GLOB SOURCE "src/*.cpp")

# Link run_tests with what we want to test and the GTest and pthread library
add_executable(gsim ${SOURCE})
target_link_libraries(gsim ${CURSES_LIBRARIES} pthread ncurses)
