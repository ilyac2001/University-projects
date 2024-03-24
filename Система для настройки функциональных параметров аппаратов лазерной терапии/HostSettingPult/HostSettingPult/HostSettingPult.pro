QT       += core gui usb concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 utf8
CONFIG += static

QMAKE_CXXFLAGS += -utf8

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    host.cpp \
    hostusbcustomhid.cpp \
    main.cpp \
    mainwindow.cpp \
    state.cpp \
    twowireinterface.cpp \
    workercommand.cpp

HEADERS += \
    host.h \
    hostusbcustomhid.h \
    mainwindow.h \
    state.h \
    twowireinterface.h \
    workercommand.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
