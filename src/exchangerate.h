#ifndef EXCHANGERATE_H
#define EXCHANGERATE_H

#include <QMutex>
#include <QDateTime>
#include <valutetype.h>
#include "vector"
#include <money.h>
#include "converterrate.h"



class ExchangeRate: protected ConverterRate
{
public:
    typedef ExchangeValue ExchangeValute;
    typedef std::vector<ExchangeValute> RateVector;
protected:
    QString bank;
    QDateTime lastUpdate_;
    static QMutex mutex;

    virtual void setting(ConvertWayLong& how, unsigned int& n, bool &isCircleError) const;
    virtual bool errorHandling(ConverterRateError er) const;
public:
    explicit ExchangeRate(QString bankName, const std::vector<ExchangeValute>& cs=std::vector<ExchangeValute>(),
                 ValuteType base_=Hryvnia);
    ExchangeRate(QString bankName,ValuteType base_);
    ExchangeRate();
    //ExchangeRate(const ExchangeRate& e);
    //ExchangeRate& operator=(const ExchangeRate& e);

    QDateTime lastUpdate() const
        { return lastUpdate_; }
    QString bankName() const
        { return bank; }
    ValuteType bankBase() const
        { return base; }
    void setBase(ValuteType t)
        { base=t; }

    bool setRate(const std::vector<ExchangeValute>& cs);
    std::vector<ValuteType> valutes() const;
    QStringList stringValutes() const;
    RateVector rateValutes() const;

    bool convert(Money from, Money& to) const;
    bool buyPrice(ValuteType what, double& result) const; //in ExchangeRate's base
    bool sellPrice(ValuteType what, double& result) const; //in ExchangeRate's base
    virtual ExchangeValute price(ValuteType what,bool& isInBank) const;

    static void lock()
    {
        mutex.lock();
    }
    static void unlock()
    {
        mutex.unlock();
    }

    virtual ~ExchangeRate()
    {
    }
};

#endif // EXCHANGERATE_H
