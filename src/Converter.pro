#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T20:02:51
#
#-------------------------------------------------
#
QT       += core gui testlib network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Converter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    exchangerate.cpp \
    money.cpp\
    converter.cpp \
    updatemanager.cpp \
    abstractupdater.cpp \
    updater.cpp \
    extractor.cpp \
    trayicon.cpp \
    splashscreen.cpp \
    convertergui.cpp \
    errormanagement.cpp \
    bankinfo.cpp \
    errormessanger.cpp \
    converterrate.cpp \
    bankdialog.cpp \
    converterform.cpp \
    rateform.cpp \
    convertertablemodel.cpp \
    privateconverter.cpp \
    paralelconverter.cpp \
    findwidget.cpp

HEADERS  += mainwindow.h \
    exchangerate.h \
    valutetype.h \
    money.h \
    converter.h \
    updatemanager.h \
    abstractupdater.h \
    updater.h \
    extractor.h \
    trayicon.h \
    splashscreen.h \
    convertergui.h \
    updateerror.h \
    errormanagement.h \
    bankinfo.h \
    errormessanger.h \
    converterrate.h \
    bankdialog.h \
    banksettings.h \
    converterform.h \
    rateform.h \
    convertertablemodel.h \
    defening.h \
    privateconverter.h \
    paralelconverter.h \
    findwidget.h


FORMS    += mainwindow.ui \
    converterform.ui

DEFINES += QT_NO_DEBUG_OUTPUT \
      QT_NO_WARNING_OUTPUT \

CONFIG += c++11
