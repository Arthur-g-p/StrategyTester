QT       += core gui
QT       += core gui charts
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/add_asset.cpp \
    Sources/addstrategy.cpp \
    Sources/apicall.cpp \
    Sources/benchmark.cpp \
    Sources/chartwindow.cpp \
    Sources/main.cpp \
    Sources/maintable.cpp \
    Sources/mainwindow.cpp \
    Sources/settingsmanager.cpp \
    Sources/status.cpp \
    Sources/stockdata.cpp \
    Sources/strategycrossing.cpp \
    Sources/subject.cpp \
    Sources/addtab.cpp \
    Sources/strategy.cpp

HEADERS += \
    Headers/add_asset.h \
    Headers/addstrategy.h \
    Headers/apicall.h \
    Headers/chartwindow.h \
    Headers/maintable.h \
    Headers/mainwindow.h \
    Headers/observer.h \
    Headers/settingsmanager.h \
    Headers/status.h \
    Headers/stockdata.h \
    Headers/subject.h \
    Headers/addtab.h \
    Headers/strategy.h \
    Headers/benchmark.h \
    Headers/strategycrossing.h

FORMS += \
    Forms/add_asset.ui \
    Forms/addstrategy.ui \
    Forms/chartwindow.ui \
    Forms/mainwindow.ui \
    Forms/moneyManagement.ui \
    Forms/strategy.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini
