cmake_minimum_required(VERSION 3.31)
project(imageconvert VERSION 0.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Build")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Qml)

set(SOURCES  src/main.cpp  src/DropIndicator.cpp)
set(HEADERS  src/DropIndicator.h)

add_executable(imageconvert WIN32            # WIN32 hides the console on Windows
        ${SOURCES} ${HEADERS}
)

if(APPLE)
    set_target_properties(imageconvert PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.selimsandal.imageconvert
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
endif()

target_link_libraries(imageconvert PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Qml
)

set_target_properties(imageconvert PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)

get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt PATHS "${QT_BIN_DIR}" NO_DEFAULT_PATH)
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET imageconvert POST_BUILD
                COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:imageconvert>"
                COMMENT "Running windeployqt"
        )
    endif()
elseif(APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt PATHS "${QT_BIN_DIR}" NO_DEFAULT_PATH)
    if(MACDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET imageconvert POST_BUILD
                COMMAND "${MACDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:imageconvert>"
                COMMENT "Running macdeployqt"
        )
    endif()
endif()
