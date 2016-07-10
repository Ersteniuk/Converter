#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>
#include <cstdint>
#include "updateerror.h"
//class AbstractUpdater;
#include "abstractupdater.h"
class QThread;
class ExchangeRate;


class UpdateManager : public QObject
{
    Q_OBJECT
private:
    AbstractUpdater* updater;
    QThread* thread;
public:
    explicit UpdateManager(AbstractUpdater* startUpdater,QObject *parent = 0);
    ~UpdateManager();

    void addRate(ExchangeRate* e);
    void addRate(const std::pair<ExchangeRate*,QString>& p)
        { emit addRateSignal(p); }
    void setRates(const std::vector<ExchangeRate*>& ps);
    void setInterval(int32_t timeInterval,bool isUpdate)
        { emit setIntervalSignal(timeInterval,isUpdate); }
signals:
    void deleteRateSignal(QString);
    void addRateSignal(ExchangeRate*,QString);
    void addRateSignal(std::pair<ExchangeRate*,QString>);
    void setRatesSignal(forQtRate);
    void setIntervalSignal(int,bool);
    void updateSignal();
    void oneSuccessUpdate();
    void oneUnSuccessUpdate();
    void beginingUpdating();
    void endUpdating();
    void warning(UpdateError);
public slots:
    void update()
        { emit updateSignal(); }

};

#endif // UPDATEMANAGER_H
