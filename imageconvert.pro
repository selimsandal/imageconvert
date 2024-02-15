QT += core gui widgets network qml quickcontrols2

CONFIG += c++26
CONFIG += autouic
CONFIG += automoc
CONFIG += autorcc

TARGET = imageconvert

# Define sources and headers
SOURCES += \
        main.cpp \
        DropIndicator.cpp

HEADERS += \
        DropIndicator.h