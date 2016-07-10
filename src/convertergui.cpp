#include <QtGlobal>
#include "convertergui.h"
#include "updatemanager.h"
#include "updater.h"
#include "splashscreen.h"
#include "trayicon.h"
#include "mainwindow.h"
#include "paralelconverter.h"
#include "exchangerate.h"
#include "QString"
#include <qstringlist.h>
#include <qeventloop.h>
#include <QFile>
#include <errormanagement.h>
#include "bankdialog.h"
#include "banksettings.h"
#include "findwidget.h"
#include <QApplication>
#include <QIcon>

#include "defening.h"
using namespace std;

UpdateManager* ConverterGui::addValutes()
try{
    //QString way=qApp->arguments().at(0);
    //QString bankInfo=way.remove(way.lastIndexOf(THIS_PROGRAM_NAME),way.size())+"bank.info";
//#ifdef Q_OS_WIN
//    QString bankInfo="\\bank.info";
//#else
//    QString bankInfo="bank.info";
//#endif
    //bank1="oschadbank",bank2="pib",bank3="aval",bank4="eximb",bank5="privatbank";
    //QStringList bank=banks();
    QString bankInfo=addressExe()+"bank.info";
    QStringList bank=settings->getCurBanks();
    quint64 deltaTime=1000*3600;

    ParalelConverter* instance=ParalelConverter::instance();
    Updater* updater=new Updater(bankInfo,deltaTime);

    for_each(bank.begin(), bank.end(), [&](const QString& s)
    {
        instance->addRate(ExchangeRate(s));
        auto tempp=instance->getRate(s);
        updater->addRate(tempp,tempp->bankName());
    });

    return new UpdateManager(updater,this);
}
catch (const UpdateError& e)
{
    criticalError(e);
}

void ConverterGui::makeConnection(UpdateManager* manager, SplashScreen* screen, TrayIcon *tray)
{
    connect(manager,SIGNAL(oneSuccessUpdate()),tray,SLOT(showMessage()));
    connect(manager,SIGNAL(oneSuccessUpdate()),screen,SLOT(update()));
    connect(manager,SIGNAL(oneUnSuccessUpdate()),screen,SLOT(update()));
    connect(manager,SIGNAL(beginingUpdating()),tray,SLOT(zeroedUpdated()));
    connect(tray,SIGNAL(updateSignal()),manager,SLOT(update()));
    connect(this,SIGNAL(updateSignal()),manager,SLOT(update()));
    connect(tray,SIGNAL(mainWindowSignal()),SLOT(createMainWindow()));
    connect(tray,SIGNAL(settingsSignal()),SLOT(settingWindow()));
    connect(window,SIGNAL(settings()),SLOT(settingWindow()));
    connect(window,SIGNAL(destroyed()),SLOT(destroyMainWindow()));
    connect(window,SIGNAL(convertError(UpdateError)),SLOT(errorHandle(UpdateError)));
    connect(this,SIGNAL(banksMayChange()),window,SIGNAL(banksMayChange()));
    connect(window,SIGNAL(updateSignal()),manager,SLOT(update()));
    connect(window,SIGNAL(find(int)),SLOT(find(int)));

    connect(manager,SIGNAL(endUpdating()),this,SLOT(endCreating()));
    connect(manager,SIGNAL(endUpdating()),this,SIGNAL(banksMayChange()));
    connect(manager,SIGNAL(warning(UpdateError)),SLOT(errorHandle(UpdateError)));

}

void ConverterGui::endCreating()
{
    loop->exit();
}

ConverterGui::ConverterGui(const QString& bankFile):programName("Converter"),banksFile(bankFile),dialog(nullptr)
{
    //bank<<"oschadbank"<<"pib"<<"aval"<<"eximb"<<"privatbank";
    QString icon1="DSC_0083.jpg",icon2="splash2.jpg";
    QString companyName="";
    QString trayIcon="icon.jpg";
    QString where="/cursy/usedBanks";
    ParalelConverter::instance(); //create not Converter, but ParalelConverter
    qApp->setWindowIcon(QIcon(addressExe()+trayIcon));

    SplashScreen screen(icon1,icon2);
    screen.show();
    screen.showMessage("Початкова ініціалізація",Qt::AlignHCenter,Qt::white);
    settings=new BankSettings(companyName,programName,where,banks(),this);
    manager=addValutes();
    TrayIcon* tray=new TrayIcon(addressExe()+trayIcon,programName,this);
    window=new MainWindow(programName,this);

    makeConnection(manager,&screen,tray);
    loop=new QEventLoop;
    screen.setMaxCount(settings->getCurBanks().count());
    screen.showMessage("Загрузка курсів валют",Qt::AlignHCenter,Qt::white);
    manager->update();
    loop->exec();
    disconnect(manager,SIGNAL(endUpdating()),this,SLOT(endCreating()));
    tray->show();
    screen.finish(window);
    window->show();
}

void ConverterGui::createMainWindow()
{

   // bank<<"oschadbank"<<"pib"<<"aval"<<"eximb"<<"privatbank";
    if (window)
    {
        window->activateWindow();
        return;
    }
    window=new MainWindow(programName,this);
    connect(window,SIGNAL(updateSignal()),manager,SLOT(update()));
    connect(window,SIGNAL(settings()),SLOT(settingWindow()));
    connect(window,SIGNAL(destroyed()),SLOT(destroyMainWindow()));
    connect(window,SIGNAL(convertError(UpdateError)),SLOT(errorHandle(UpdateError)));
    connect(this,SIGNAL(banksMayChange()),window,SIGNAL(banksMayChange()));
    connect(window,SIGNAL(find(int)),SLOT(find(int)));
    window->show();
}

QStringList ConverterGui::banks()
try
{
    QStringList sl;
    QString line;
    //QFile f("bankList.info");
    QFile f(banksFile);
    f.open(QFile::ReadOnly);
    ErrorManagement::FileChecker fCheck(f);
    while (!f.atEnd())
    {
        line=f.readLine();
        if (f.atEnd() && line.isEmpty())
            break;
        fCheck.check(false,UpdateError::critical);
        auto it=line.end();
        while ((--it)->isSpace())
        {
            line.remove(line.size()-1,1);
            it=line.end();
        }
        sl<<line;
    }
    return sl;

}
catch (const UpdateError& e)
{
    errorHandle(e);
}
catch (...)
{
    throw StopKran();
}

void ConverterGui::settingWindow()
{
    if (!dialog)
    {
        dialog=new BankDialog("Вибір валют",banks(),ParalelConverter::instance()->banks());
        connect(dialog,SIGNAL(newBank(QStringList,bool)),SLOT(newBanks(QStringList,bool)));
        connect(dialog,SIGNAL(destroyed()),SLOT(destroyDialog()));
        dialog->show();
    }
    dialog->activateWindow();
}

void ConverterGui::newBanks(const QStringList &sl, bool shouldBeUpdated)
{
    auto pc=ParalelConverter::instance();
    pc->createRates(sl);
    settings->saveCurBanks(sl);
    manager->setRates(pc->getRates());
    emit banksMayChange();
    if (shouldBeUpdated)
        emit updateSignal();
}

void ConverterGui::find(int i)
try{
    FindWidget* fw=nullptr;
    MainWindow::FindKind kind=static_cast<MainWindow::FindKind>(i);
    switch (kind)
    {
    case MainWindow::maxSellType:
        fw=new FindWidget(&Converter::maxSellPrice,"Максимальна вартість продажу");
        break;
    case MainWindow::maxBuyType:
        fw=new FindWidget(&Converter::maxBuyPrice,"Максимальна вартість купівлі");
        break;
    case MainWindow::minSellType:
        fw=new FindWidget(&Converter::minSellPrice,"Мінімальна вартість продажу");
        break;
    case MainWindow::minBuyType:
        fw=new FindWidget(&Converter::minBuyPrice,"Мінімальна вартість купівлі");
        break;
    default:
        ErrorManagement::error("Пошук","Цю проблему згодом буде налагоджено."
                               " Вибачте, будь-ласка, за тимчасові незручності.");
    }
    fw->show();
}
catch(UpdateError e) {
    errorHandle(e);
}
catch (...)
{
    throw StopKran();
}
