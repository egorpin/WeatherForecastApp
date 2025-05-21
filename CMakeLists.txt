cmake_minimum_required(VERSION 3.16)
project(WeatherApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Gui)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found! Please install Qt6 development packages.")
endif()

file(GLOB SOURCES
    "*.cpp"
    "WeatherAPI/*.cpp"
    "WeatherAPI/*.hpp"
    "MainWindow/*.cpp"
    "MainWindow/*.hpp"
    "Utilities/*.cpp"
    "Utilities/*.hpp"
)


add_executable(WeatherApp ${SOURCES})

qt_add_resources(WeatherApp "icons"
    PREFIX "/"
    FILES
        MainWindow/icons/minimize.png
        MainWindow/icons/maximize.png
        MainWindow/icons/close.png
        MainWindow/icons/restore.png
        MainWindow/icons/app_icon.png
        MainWindow/icons/search.png
        MainWindow/icons/error.png
        MainWindow/icons/humidity.png
        MainWindow/icons/wind.png
)

qt_add_resources(WeatherApp "images"
    PREFIX "/"
    FILES
        MainWindow/resources/backgrounds/clear.jpg
        MainWindow/resources/backgrounds/few_clouds.jpg
        MainWindow/resources/backgrounds/scattered_clouds.jpg
        MainWindow/resources/backgrounds/broken_clouds.jpg
        MainWindow/resources/backgrounds/overcast.jpg
        MainWindow/resources/backgrounds/drizzle.jpg
        MainWindow/resources/backgrounds/rain.jpg
        MainWindow/resources/backgrounds/rain_snow.jpg
        MainWindow/resources/backgrounds/snow.jpg
        MainWindow/resources/backgrounds/mist.jpg
        MainWindow/resources/backgrounds/default.jpg
)

target_link_libraries(WeatherApp PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
)
