QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET      = roundcircle
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += frmroundcircle.cpp
SOURCES     += roundcircle.cpp

HEADERS     += frmroundcircle.h
HEADERS     += roundcircle.h

FORMS       += frmroundcircle.ui
