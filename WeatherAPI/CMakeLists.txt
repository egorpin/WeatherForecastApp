cmake_minimum_required(VERSION 3.23)

set(project "WeatherAPI")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        WeatherAPI.cpp WeatherObject.cpp)

set(${project}_HEADERS
WeatherAPI.hpp WeatherObject.hpp)

target_link_libraries(${project}
        Qt6::Widgets
        Qt6::Network
)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})
