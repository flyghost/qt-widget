QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET      = codetool
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmcodetool.cpp
HEADERS     += frmcodetool.h
FORMS       += frmcodetool.ui

