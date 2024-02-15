cmake_minimum_required(VERSION 3.27)

project(imageconvert)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        Qml
        QuickControls2
        )

qt_standard_project_setup()




### detect cpu arch
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )
### end detect cpu arch

if(WIN32)
    qt_add_executable(imageconvert WIN32 main.cpp
            DropIndicator.cpp
            DropIndicator.h
    )
endif()

if(LINUX)
    message("running on linux")
    qt_add_executable(imageconvert main.cpp
            DropIndicator.cpp
            DropIndicator.h
    )
endif ()

if(APPLE)
    qt_add_executable(imageconvert MACOSX_BUNDLE main.cpp
            DropIndicator.cpp
            DropIndicator.h
    )

    ### build universal binary if on x86_64 mac
    if (${ARCHITECTURE} STREQUAL "x86_64")
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
    endif ()
endif()

target_link_libraries(imageconvert PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Network
        Qt::Qml
        Qt::QuickControls2
)


