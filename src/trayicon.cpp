#include "trayicon.h"
#include <QMenu>
#include <QApplication>

TrayIcon::TrayIcon(const QString& trayIcon,const QString& programName, QObject *parent) :
    QSystemTrayIcon(QIcon(trayIcon),parent)
{
    setToolTip(programName);

    pMenu=new QMenu;
    pMenu->addAction("Головне вікно",this,SIGNAL(mainWindowSignal()));
    pMenu->addAction("Вікно налаштувань",this,SIGNAL(settingsSignal()));
    pMenu->addAction("Оновити курси",this,SIGNAL(updateSignal()));
    pMenu->addSeparator();
    pMenu->addAction("Вийти",qApp,SLOT(quit()));
    setContextMenu(pMenu);

    connect(this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(activatedSlot(QSystemTrayIcon::ActivationReason)));
}

void TrayIcon::showMessage()
{
    QSystemTrayIcon::showMessage("Курси валют","Оновлено "+QString::number(++updated)+" курсів валют.",
                QSystemTrayIcon::Information,1500);
}

TrayIcon::~TrayIcon()
{
    delete pMenu;
}
