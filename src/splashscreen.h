#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
private:
    bool second;
    int rateDownloaded;
    QString icon2Name;
    qint32 maxCount;
public:
    SplashScreen(const QString &icon1, const QString icon2);
    void setMaxCount(qint32 maxNumber)
        { maxCount=maxNumber; }
public slots:
    void update();
};

#endif // SPLASHSCREEN_H
