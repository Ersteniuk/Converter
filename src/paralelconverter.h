#ifndef PARALELCONVERTER_H
#define PARALELCONVERTER_H
#include "converter.h"

#include <QDebug>
#include <QObject>
#include <QWaitCondition>
#include <stdexcept>

class ParalelConverter :public QObject, public Converter
{
    Q_OBJECT
private:
    mutable bool toLock;
    mutable QWaitCondition deleteCondition;
    mutable QWaitCondition updateStartCondition;

    ParalelConverter():toLock(false)
    {}

public:
    mutable QMutex deleteMutex;
    mutable QMutex updateStart;

    static ParalelConverter* instance();

    bool addRate(const ExchangeRate& e);
    double buyPrice(ValuteType valute, bool& isFinded,QString bankName="") const;
    double minBuyPrice(ValuteType valute, bool& isFinded,ValuteType baseValute=Hryvnia,
                       QString* bank=nullptr) const;
    double maxBuyPrice(ValuteType valute, bool& isFinded,ValuteType baseValute=Hryvnia,
                       QString* bank=nullptr) const;
    double sellPrice(ValuteType valute, bool& isFinded, QString bankName="") const;
    double minSellPrice(ValuteType valute, bool& isFinded,ValuteType baseValute=Hryvnia,
                        QString* bank=nullptr) const;
    double maxSellPrice(ValuteType valute, bool& isFinded,ValuteType baseValute=Hryvnia,
                        QString* bank=nullptr) const;
    bool convert(Money from, Money& to, QString bankName="") const;
    bool deleteRate(const QString& bankName,bool isNeeds);
    void clearRates(bool isNeeds);
    ExchangeValutes valutes(const QString &bankName="") const;
    QStringList stringValutes(const QString& bankName="") const;

    void deleteWakeAll() const
        { deleteCondition.wakeAll(); }
    void deleteWakeOne() const
        { deleteCondition.wakeOne(); }
    bool waitUpdateStart(unsigned long time = ULONG_MAX) const
        { return updateStartCondition.wait(&updateStart,time); }
signals:
    void wantDelete(QString);
    void wantDeleteAll();
public slots:
    void setLocker(bool isLock) const
    {

        toLock=isLock;
        if (isLock)
        {
            updateStart.lock();
            updateStart.unlock();
            updateStartCondition.wakeAll();
        }
    }
};

#endif // PARALELCONVERTER_H
