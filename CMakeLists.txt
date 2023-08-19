cmake_minimum_required(VERSION 3.26)
project(ParsTools)

set(CMAKE_CXX_STANDARD 23)

add_executable(ParsTools
        main.cpp
        utils.hpp
        utils.cpp
        parser_base.hpp
        parsers.hpp)
