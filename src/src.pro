#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T23:08:20
#
#-------------------------------------------------

QT       += core gui phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlivestreamer
TEMPLATE = app

qbinary.path = $${PREFIX}/bin
qbinary.files = qlivestreamer

qshare.path = $${PREFIX}/share/qlivestreamer/
qshare.files = images/qlivestreamer.png

qdesktop.path = $${PREFIX}/share/applications/
qdesktop.files = files/qlivestreamer.desktop

INSTALLS += qbinary qshare qdesktop


SOURCES += main.cpp\
        mainwindow.cpp \
    bookmark.cpp

HEADERS  += mainwindow.h \
    bookmark.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

