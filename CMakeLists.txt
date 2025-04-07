cmake_minimum_required(VERSION 3.16)
project(WeatherApp)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(WeatherApp main.cpp)
qt_add_resources(WeatherApp "resourses"
    PREFIX "/"
    FILES
        icons/app_icon.png
        icons/search.png
)

# Link Qt libraries
target_link_libraries(WeatherApp
    Qt6::Widgets
    Qt6::Network
)

# Enable automatic moc, uic and rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(ENABLE_DEBUG)
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()
