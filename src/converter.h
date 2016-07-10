#ifndef CONVERTER_H
#define CONVERTER_H
#include "privateconverter.h"
#include <vector>

#include <iostream>

//клас розрахований на оновлення інформації курсів і часу останнього оновлення в іншому потоці,але відносно
//імені банку і самого-себе не є thread-safe
class Converter:protected PrivateConverter
{
protected:
    typedef std::vector<ExchangeRate*> pRates;
    typedef std::vector<const ExchangeRate*> const_pRates;
    typedef std::vector<ExchangeValue> ExchangeValutes;

    Converter()
    {}

    iterator find(QString bankName);
    const_iterator find(QString bankName) const;

    static Converter* obj;

    QString defaultBank_;
public:

    static Converter* instance()
    {
        if (!obj)
            obj=new Converter();
        return obj;
    }

    void setDefaultBank(QString name="")
        { defaultBank_=name; }
    QString defaultBank() const
        { return defaultBank_; }

    virtual bool addRate(const ExchangeRate &e);
    virtual bool deleteRate(const QString &bankName, bool isNeeds=true);
    virtual void clearRates(bool isNeeds=true);

    virtual void createRates(const QStringList& sl);

    virtual  pRates getRates();
    virtual const_pRates getRates() const;
    virtual bool checkRate(QString bankName="") const;
    virtual ExchangeRate* getRate(QString bankName="");
    virtual const ExchangeRate* getRate(QString bankName="") const;
    virtual ExchangeRate* operator[] (QString bankName);
    virtual const ExchangeRate* operator[] (QString bankName) const;

    virtual double buyPrice(ValuteType valute, bool& isFinded, QString bankName="") const;
    virtual double minBuyPrice(ValuteType valute, bool& isFinded, ValuteType baseValute=Hryvnia,
                               QString* bank=nullptr) const;
    virtual double maxBuyPrice(ValuteType valute, bool& isFinded, ValuteType baseValute=Hryvnia,
                               QString* bank=nullptr) const;
    virtual double sellPrice(ValuteType valute, bool& isFinded, QString bankName="") const;
    virtual double minSellPrice(ValuteType valute, bool& isFinded, ValuteType baseValute=Hryvnia,
                                QString* bank=nullptr) const;
    virtual double maxSellPrice(ValuteType valute, bool& isFinded, ValuteType baseValute=Hryvnia,
                                QString* bank=nullptr) const;

    virtual bool convert(Money from, Money& to,QString bankName="") const;
    virtual ExchangeValutes valutes(const QString& bankName="") const;
    virtual QStringList stringValutes(const QString& bankName="") const;

    virtual QStringList banks() const;
};

#endif // CONVERTER_H
