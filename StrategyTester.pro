QT       += core gui
QT       += core gui charts
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_asset.cpp \
    apicall.cpp \
    charts.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsmanager.cpp \
    status.cpp \
    stockdata.cpp \
    subject.cpp

HEADERS += \
    add_asset.h \
    apicall.h \
    charts.h \
    mainwindow.h \
    observer.h \
    settingsmanager.h \
    status.h \
    stockdata.h \
    subject.h

FORMS += \
    add_asset.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Ressources.qrc

DISTFILES += \
    config.ini
