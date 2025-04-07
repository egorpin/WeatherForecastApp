cmake_minimum_required(VERSION 3.23)

set(project "MainWindow")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        MainWindow.cpp)

set(${project}_HEADERS
MainWindow.hpp)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})
