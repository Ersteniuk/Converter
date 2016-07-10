#include "splashscreen.h"
#include <QApplication>
#include <QDesktopWidget>
#include "defening.h"

SplashScreen::SplashScreen(const QString& icon1, const QString icon2) :
    second(false), rateDownloaded(0),icon2Name(icon2),maxCount(0)
{
//#ifdef Q_OS_WIN
//    if (! icon1.startsWith('/'))
//        icon1.prepend('/');
//#endif
    QPixmap pix(addressExe()+icon1);
    setPixmap(pix.scaled(QApplication::desktop()->screenGeometry().size(),Qt::KeepAspectRatio));
    showMessage("Ініціалізація головного вікна",Qt::AlignHCenter);
}

void SplashScreen::update()
{
    if (!second)
    {
//#ifdef Q_OS_WIN
//        if (! icon2Name.startsWith('\\'))
//            icon2Name.prepend('\\');
//#endif
        setPixmap(QPixmap(addressExe()+icon2Name));
        second=true;
    }
    showMessage(QString("Загружено курсв: "+QString::number(++rateDownloaded)+" із %1").arg(maxCount),Qt::AlignHCenter);
}
