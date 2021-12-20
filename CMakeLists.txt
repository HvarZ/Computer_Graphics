cmake_minimum_required(VERSION 3.17)
project(computer_graphic)

set(CMAKE_CXX_STANDARD 17)

add_executable(line homework_1/line.cpp)
add_executable(Cyrus_Beck homework_2/Cyrus_Beck_algorithm.cpp)
add_executable(filling_convex homework_5/filling_convex.cpp)
add_executable(bezier_rotating homework_6/bezier_rotating.cpp)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
find_package(OpenCV COMPONENTS core highgui REQUIRED)

target_link_libraries(line sfml-graphics sfml-audio)
target_link_libraries(Cyrus_Beck sfml-graphics sfml-audio)
target_link_libraries(filling_convex opencv_core opencv_highgui)
target_link_libraries(bezier_rotating opencv_core opencv_highgui)