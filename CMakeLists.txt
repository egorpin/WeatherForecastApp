cmake_minimum_required(VERSION 3.16)
project(WeatherApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Gui Positioning)
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

qt_add_resources(WeatherApp "backgrounds"
    PREFIX "/"
    FILES
        MainWindow/resources/backgrounds/bg1.jpg
        MainWindow/resources/backgrounds/bg2.jpg
        MainWindow/resources/backgrounds/bg3.jpg
)

qt_add_resources(WeatherApp "styles"
    PREFIX "/"
    FILES
        MainWindow/resources/styles/backgrounds.qss
)


# target_include_directories(WeatherApp PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/WeatherAPI
#     ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
#     ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow
# )

# # Подключение библиотек Qt
target_link_libraries(WeatherApp PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
    Qt6::Positioning
)


# cmake_minimum_required(VERSION 3.16)
# project(WeatherApp)

# find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# file(GLOB SOURCES
#     "*.cpp"
#     "WeatherAPI/*.cpp"
#     "WeatherAPI/*.hpp"
#     "MainWindow/*.cpp"
#     "MainWindow/*.hpp"
#     "Utilities/*.cpp"
#     "Utilities/*.hpp"
# )

# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )


# add_executable(WeatherApp ${SOURCES})
# qt_add_resources(WeatherApp "MainWindow/resourses.qrc"
#     PREFIX "/"
#     FILES
#         # MainWindow/icons/app_icon.png
#         # MainWindow/icons/search.png
#         MainWindow/icons/minimize.png
#         MainWindow/icons/maximize.png
#         MainWindow/icons/close.png
#         MainWindow/icons/restore.png
# )

# qt_add_resources(WeatherApp "MainWindow/resources/backgrounds.qrc"
#     PREFIX "/"
#     FILES
#         MainWindow/resources/backgrounds/bg1.jpg
#         MainWindow/resources/backgrounds/bg2.jpg
#         MainWindow/resources/backgrounds/bg3.jpg
#         MainWindow/resources/styles/backgrounds.qss
# )
# find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Network)
# target_link_libraries(WeatherApp
#     Qt6::Widgets
#     Qt6::Network
#     Qt6::Gui
# )
