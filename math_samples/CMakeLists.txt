cmake_minimum_required(VERSION 3.30)
project(math_samples)

set(CMAKE_CXX_STANDARD 20)
include_directories("../mpl1")
link_directories("../mpl1/cmake-build-debug")
add_executable(math_samples main.cpp)
target_link_libraries(math_samples defmath meval)
