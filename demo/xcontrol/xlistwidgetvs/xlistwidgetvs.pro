QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET      = xlistwidgetvs
TEMPLATE    = app
DESTDIR     = $$PWD/../bin
CONFIG      += warn_off

SOURCES     += main.cpp
SOURCES     += xlistwidgetvs.cpp
SOURCES     += frmxlistwidgetvs.cpp
HEADERS     += frmxlistwidgetvs.h
HEADERS     += xlistwidgetvs.h
FORMS       += frmxlistwidgetvs.ui
