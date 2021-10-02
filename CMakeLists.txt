cmake_minimum_required(VERSION 3.17)
project(computer_graphic)

set(CMAKE_CXX_STANDARD 17)

add_executable(computer_graphic homework_1/line.cpp)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

target_link_libraries(computer_graphic sfml-graphics sfml-audio)