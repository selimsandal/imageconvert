QT += core gui widgets network qml quickcontrols2

CONFIG += c++26

TARGET = imageconvert

VERSION=0.0.3

SHORT_VERSION = 0.0.3
FULL_VERSION = 0.0.3

# Define sources and headers
SOURCES += \
        src/main.cpp \
        src/DropIndicator.cpp

HEADERS += \
        src/DropIndicator.h

ICON = icon.icns