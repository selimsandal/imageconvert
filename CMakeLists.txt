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


get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")



### detect cpu arch
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )
### end detect cpu arch

if(WIN32)
    qt_add_executable(imageconvert WIN32 main.cpp
            DropIndicator.cpp
            DropIndicator.h
    )

    set_target_properties(WorkTracker PROPERTIES
            WIN32_EXECUTABLE ON
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

    set_target_properties(imageconvert PROPERTIES
            MACOSX_BUNDLE TRUE
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

install(TARGETS imageconvert
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
        TARGET imageconvert
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})



if(APPLE)
    add_custom_command(TARGET imageconvert POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            ARGS "imageconvert.app" "-dmg"
            COMMENT "Execute macdeployqt to create macOS bundle")
endif ()
