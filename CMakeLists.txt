# GTP Simulator cmake file
cmake_minimum_required(VERSION 2.8.9)
project(gsim)

include(ExternalProject)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra \
    -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef \
    -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter"
)

ExternalProject_Add(cxxopts
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/cxxopts
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../3rdparty/cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG "v2.1.1"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX=${CMAKE_CURRENT_BINARY_DIR}/cxxopts
)

find_package(Curses REQUIRED)

include_directories(
    src
    3rdparty/cxxopts/include
    ${CURSES_INCLUDE_DIR}
)

file(GLOB SOURCE "src/*.cpp")

# Link run_tests with what we want to test and the GTest and pthread library
add_executable(gsim ${SOURCE})
add_dependencies(gsim cxxopts)
target_link_libraries(gsim ${CURSES_LIBRARIES} pthread ncurses)
