#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T20:02:51
#
#-------------------------------------------------
#
QT       += core gui testlib network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Converter
TEMPLATE = app console


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
    findwidget.cpp \
    savemanager.cpp \
    saver.cpp \
    SPLib/BasicExcel.cpp \
    SPLib/ByteArray.cpp \
    SPLib/CellImpl.cpp \
    SPLib/CellsImpl.cpp \
    SPLib/ColumnImpl.cpp \
    SPLib/ColumnsImpl.cpp \
    SPLib/Date.cpp \
    SPLib/ExcelUtil.cpp \
    SPLib/ExceptionImpl.cpp \
    SPLib/Formulas.cpp \
    SPLib/IllegalArgumentException.cpp \
    SPLib/IllegalStateException.cpp \
    SPLib/IndexLimits.cpp \
    SPLib/ioapi.cpp \
    SPLib/IOException.cpp \
    SPLib/OdsWriter.cpp \
    SPLib/OdsWriterImpl.cpp \
    SPLib/RowImpl.cpp \
    SPLib/RowsImpl.cpp \
    SPLib/splib.cpp \
    SPLib/SpreadsheetImpl.cpp \
    SPLib/Strings.cpp \
    SPLib/TableImpl.cpp \
    SPLib/Time.cpp \
    SPLib/ToUTF8.cpp \
    SPLib/ToUTF16.cpp \
    SPLib/Util.cpp \
    SPLib/XlsWriter.cpp \
    SPLib/XlsWriterImpl.cpp \
    SPLib/XlsxWriter.cpp \
    SPLib/XlsxWriterImpl.cpp \
    SPLib/zip.cpp \
    SPLib/ZipArchive.cpp

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
    findwidget.h \
    savemanager.h \
    saver.h \
    SPLib/BasicExcel.h \
    SPLib/ByteArray.h \
    SPLib/CellImpl.h \
    SPLib/CellsImpl.h \
    SPLib/ColumnImpl.h \
    SPLib/ColumnsImpl.h \
    SPLib/ExcelUtil.h \
    SPLib/Formulas.h \
    SPLib/IndexedCollectionImpl.h \
    SPLib/IndexLimits.h \
    SPLib/ioapi.h \
    SPLib/OdsWriterImpl.h \
    SPLib/RowImpl.h \
    SPLib/RowsImpl.h \
    SPLib/splib.h \
    SPLib/splibint.h \
    SPLib/Strings.h \
    SPLib/TableImpl.h \
    SPLib/ToUTF8.h \
    SPLib/ToUTF16.h \
    SPLib/Util.h \
    SPLib/XlsWriterImpl.h \
    SPLib/XlsxWriterImpl.h \
    SPLib/zconf.h \
    SPLib/zip.h \
    SPLib/ZipArchive.h \
    SPLib/zlib.h


FORMS    += mainwindow.ui \
    converterform.ui

#DEFINES += QT_NO_DEBUG_OUTPUT \
#      QT_NO_WARNING_OUTPUT \

CONFIG += c++11

OTHER_FILES += \
    SPLib/CMakeLists.txt

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += zlib

win32: LIBS += -L$$PWD/../build-Converter-Desktop_Qt_5_2_1_MinGW_32bit-Debug/ -lzdll -liconv

INCLUDEPATH += $$PWD/../build-Converter-Desktop_Qt_5_2_1_MinGW_32bit-Debug
DEPENDPATH += $$PWD/../build-Converter-Desktop_Qt_5_2_1_MinGW_32bit-Debug
