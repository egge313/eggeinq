#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T19:17:37
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = eggeinq
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

greaterThan(QT_VERSION, 6) {
   message("Eggeinq warning: QT API version 5.9 required.")
}


CONFIG += c++20

SOURCES +=              \
        main.cpp        \
        mainwindow.cpp  \
        curlget.cpp     \
        osrelease.cpp   \
        workerthread.cpp

HEADERS +=              \
        mainwindow.h    \
        curlget.h       \
        osrelease.h     \
        workerthread.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L/usr/lib/ -L/usr/lib/x86_64-linux-gnu -lcurl -lGL

INCLUDEPATH += /usr/inluce/x86_64-linux-gnu
INCLUDEPATH += /usr/include/c++/6

DEPENDPATH += /usr/inluce/x86_64-linux-gnu

DISTFILES += \
    qss/coffee.qss \
    qss/esa-orig.qss

RESOURCES += \
    resource.qrc
