#include "paralelconverter.h"
#include "defening.h"
#include <cassert>
#include <QApplication>
#include <typeinfo>

ParalelConverter* ParalelConverter::instance()
{
    if (!obj)
        obj=new ParalelConverter;
    ParalelConverter* c=dynamic_cast<ParalelConverter*>(obj);
    assert(c);
    if (!c)
       throw std::bad_cast();
    return c;
}

bool ParalelConverter::addRate(const ExchangeRate& e)
{
    if (e.bankName()=="")
            return false;
    auto it=find(e.bankName());
    if (cursy.end()==it)
    {
        cursy.push_back(e);
    }
    else
    {
        if (toLock)
            it->lock();
        *it=e;
        if (toLock)
            it->unlock();
    }
    return true;
}

double ParalelConverter::buyPrice(ValuteType valute, bool& isFinded,QString bankName) const
{
    if (toLock)
        ExchangeRate::lock();
    double returning=Converter::buyPrice(valute, isFinded, bankName);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

double ParalelConverter::minBuyPrice(ValuteType valute, bool& isFinded,ValuteType baseValute, QString* bank) const
{
    if (toLock)
        ExchangeRate::lock();
    double returning=Converter::minBuyPrice(valute,isFinded, baseValute,bank);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

double ParalelConverter::maxBuyPrice(ValuteType valute, bool& isFinded,ValuteType baseValute, QString* bank) const
{
    if (toLock)
        ExchangeRate::lock();
    double returning=Converter::maxBuyPrice(valute,isFinded, baseValute,bank);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

double ParalelConverter::minSellPrice(ValuteType valute, bool& isFinded,ValuteType baseValute, QString* bank) const
{
    if (toLock)
        ExchangeRate::lock();
    double returning=Converter::minSellPrice(valute,isFinded, baseValute,bank);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

double ParalelConverter::maxSellPrice(ValuteType valute, bool& isFinded, ValuteType baseValute, QString *bank) const
{
    if (toLock)
        ExchangeRate::lock();
    double returning=Converter::maxSellPrice(valute,isFinded, baseValute,bank);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

double ParalelConverter::sellPrice(ValuteType valute, bool& isFinded, QString bankName) const
{
    if (toLock)
        ExchangeRate::lock();
    double returning=Converter::sellPrice(valute, isFinded,  bankName);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

bool ParalelConverter::convert(Money from, Money& to, QString bankName) const
{
    if (toLock)
        ExchangeRate::lock();
    bool returning=Converter::convert(from, to, bankName);
    if (toLock)
        ExchangeRate::unlock();
    return returning;
}

bool ParalelConverter::deleteRate(const QString& bankName,bool isNeeds)
{
    auto it=find(bankName);
    if (cursy.end()==it)
        return false;
    while(true)
    {
        deleteMutex.lock();
        emit wantDelete(bankName);
        bool endBlocking=deleteCondition.wait(&deleteMutex,500);
        deleteMutex.unlock();
        if (endBlocking)
        {
            cursy.erase(it);
            return true;
        }
        if (!isNeeds)
            ErrorManagement::error("Changing exchange rates","Deleting some exchange rates was unsucsessful",
                                   UpdateError::huge);
        qApp->processEvents();
    }
}

void ParalelConverter::clearRates(bool isNeeds)
{
    while(true)
    {
        deleteMutex.lock();
        emit wantDeleteAll();
        bool endBlocking=deleteCondition.wait(&deleteMutex,500);
        deleteMutex.unlock();
        if (endBlocking)
        {
            cursy.clear();
            return;
        }
        if (!isNeeds)
            ErrorManagement::error("Changing exchange rates","Deleting some exchange rates was unsucsessful",
                               UpdateError::huge);
        qApp->processEvents();
    }
}

Converter::ExchangeValutes ParalelConverter::valutes(const QString &bankName) const
{
    auto it=find(bankName);
    if (cursy.end()==it)
        return Converter::ExchangeValutes();
    if (toLock)
        it->lock();
    Converter::ExchangeValutes returning=it->rateValutes();
    if (toLock)
        it->unlock();
    return returning;
}

QStringList ParalelConverter::stringValutes(const QString& bankName) const
{
    auto it=find(bankName);
    if (cursy.end()==it)
        return QStringList();
    if (toLock)
        it->lock();
    QStringList returning=it->stringValutes();
    if (toLock)
        it->unlock();
    return returning;
}
