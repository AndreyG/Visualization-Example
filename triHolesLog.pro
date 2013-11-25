TEMPLATE = app
TARGET = triHolesLog

CONFIG += QtGui
QT += opengl

OBJECTS_DIR = bin

QMAKE_CXXFLAGS = -std=c++11 -Wall -g
QMAKE_CXXFLAGS -= -O2

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
           headers/algorithms_geom.h \
           headers/app_viewer.h \
           headers/polygon_triangulator.h \
           headers/triangulation_types.h \
           headers/triangulation_status.h \
           headers/drawer.h \
           headers/segment_map.h \

SOURCES += src/main.cpp \
           src/algorithms_geom.cpp \ 
           src/app_viewer.cpp \
           src/polygon_triangulator.cpp \
           src/triangulation_types.cpp \
           src/triangulation_status.cpp \

LIBS += -Lvisualization -lvisualization
