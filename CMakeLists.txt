cmake_minimum_required(VERSION 3.27)
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


if(WIN32)
    add_executable(imageconvert WIN32 main.cpp)
else()
    add_executable(imageconvert MACOSX_BUNDLE main.cpp)
endif()

target_link_libraries(imageconvert
        Qt::Core
        Qt::Gui
        Qt::Widgets
)


