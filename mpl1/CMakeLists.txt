cmake_minimum_required(VERSION 3.30)
project(mpl1)

set(CMAKE_CXX_STANDARD 20)

add_library(meval STATIC meval.cpp
        meval.h
        )

add_library(defmath STATIC def_math.cpp
        def_math.h
        meval.h
)

target_link_libraries(defmath meval)