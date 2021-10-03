cmake_minimum_required(VERSION 3.17)
project(computer_graphic)

set(CMAKE_CXX_STANDARD 17)

add_executable(line homework_1/line.cpp)
add_executable(Cyrus_Beck homework_2/Cyrus_Beck_algorithm.cpp)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

target_link_libraries(line sfml-graphics sfml-audio)
target_link_libraries(Cyrus_Beck sfml-graphics sfml-audio)