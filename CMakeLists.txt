cmake_minimum_required(VERSION 3.16)
project(WeatherApp)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES
    "*.cpp"
    "MainWindow/*.cpp"
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(WeatherApp ${SOURCES})
qt_add_resources(WeatherApp "MainWindow/resourses.qrc"
    PREFIX "/"
    FILES
        MainWindow/icons/app_icon.png
        MainWindow/icons/search.png
)

target_link_libraries(WeatherApp
    Qt6::Widgets
    Qt6::Network
)
