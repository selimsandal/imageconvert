cmake_minimum_required(VERSION 3.28)
project(imageconvert)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(imageconvert main.cpp)
target_link_libraries(imageconvert
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

