TEMPLATE = app
TARGET = triHolesLog

CONFIG += QtGui
QT += opengl

OBJECTS_DIR = bin

QMAKE_CXXFLAGS = -std=c++11 -Wall -g

macx {
    QMAKE_CXXFLAGS += -stdlib=libc++  
    QMAKE_LFLAGS += -lc++
}

CONFIG += precompile_header

DEPENDPATH += src \
              visualization/headers \
              visualization/headers/common \
              visualization/headers/io \
              visualization/headers/visualization \

INCLUDEPATH += src \
               visualization/headers \
               headers \

HEADERS += headers/stdafx.h \
           headers/algorithms.h \
           headers/app_viewer.h \

SOURCES += src/main.cpp \
           src/algorithms.cpp \ 
           src/app_viewer.cpp \

LIBS += -Lvisualization -lvisualization
