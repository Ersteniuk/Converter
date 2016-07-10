#ifndef UPDATER_H
#define UPDATER_H

#include <string>
#include "abstractupdater.h"
#include "valutetype.h"
#include <QUrl>
#include <iostream>
#include "exchangerate.h"
#include <qtextstream.h>
#include "bankinfo.h"
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class  AbstractExtractor;
class QSslError;

class Updater : public AbstractUpdater
{
    Q_OBJECT
private:
    typedef BankInfo Info;
    typedef std::vector<BankInfo> Infos;
    typedef Infos::iterator InfoIterator;
    typedef Infos::const_iterator InfoCIterator;

    QNetworkAccessManager* loader;
    Infos infos;
    ValuteType base;

    InfoIterator getBankInfo(const QString &s);
    InfoIterator getBankInfo(const QUrl &s);
public:
    Updater(QString infoFile,int32_t timeInterval,ValuteType baseType=Hryvnia, QObject *parent = 0);
    ~Updater()
        { zeroedCount(); }
protected:
       void zeroedCount() noexcept;
       bool processOne(const Rate& e);

protected slots:
    virtual void endWork(QNetworkReply* r);
    void sslError(QList<QSslError>);//easy procesing-just ignoring(and in debug mode printing into console)
};

#endif // UPDATER_H
