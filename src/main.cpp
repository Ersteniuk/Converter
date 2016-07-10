#include "mainwindow.h"
#include "trayicon.h"
#include <QApplication>
#include <QMutex>
//#include <iostream>
#include <typeinfo>
#include "convertergui.h"
#include "updateerror.h"
#include "QDebug"
#include "exchangerate.h"
#include "abstractupdater.h"

#include <QMenu>
#include "defening.h"

int main(int argc, char *argv[])
try{
    qDebug()<<argv[0];
    qRegisterMetaType<UpdateError>("UpdateError");
    //qRegisterMetaType<std::vector<std::pair<ExchangeRate*,QString> >>("Updating vector");
    qRegisterMetaType<forQtRate>("Updating vector");
    QApplication a(argc, argv);
    //MainWindow w;
    //TrayIcon w("Converter");
    QApplication::setQuitOnLastWindowClosed(false);
//#ifdef Q_OS_WIN
//   ConverterGui w("\\bankList.info");
//#else
    ConverterGui w(addressExe()+"bankList.info");
//#endif

    //QObject::connect(&w,SIGNAL(mainWindowSignal()),&a,SLOT(quit()));
    //w.show();

    return a.exec();
}
catch (std::bad_alloc)
{
    return 1;
}
catch (StopKran)
{
    return 2;
}
catch (std::bad_cast)
{
    qDebug()<<"bad cast exception";
    return 3;
}
catch (...)
{
    qDebug()<<"unknow exception";
    return -1;
}

QString addressExe()
{
    static QString way=qApp->arguments().at(0);
    static QString wantedAdress=way.remove(way.lastIndexOf(THIS_PROGRAM_NAME_FOR_SHITING_WINDOWS),
                                           way.size());
    return wantedAdress;
}
