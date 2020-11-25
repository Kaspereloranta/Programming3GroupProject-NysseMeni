QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_statisticstests.cpp\
        ../Game/statistics.cpp

HEADERS += \
        ../Game/statistics.hh

INCLUDEPATH += \
            ../Game\
            ../CourseLib
