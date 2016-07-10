#include "converter.h"
#include <algorithm>
#include "errormanagement.h"
using namespace std;

Converter* Converter::obj=0;

Converter::iterator Converter::find(QString bankName)
{
    if (""==bankName)
    {
        if (""==defaultBank_)
            return cursy.begin();
        else
            bankName=defaultBank_;
    }
    return PrivateConverter::find(bankName);
}

Converter::const_iterator Converter::find(QString bankName) const
{
    if (""==bankName)
    {
        if (""==defaultBank_)
            return cursy.cbegin();
        else
            bankName=defaultBank_;
    }
    return PrivateConverter::find(bankName);
}

Converter::pRates Converter::getRates()
{
    pRates ps(cursy.size());
    auto it=ps.begin();
    for_each(cursy.begin(), cursy.end(), [&](ExchangeRate& e)
    {
        *(it++)=&e;
    });
    return ps;
}

Converter::const_pRates Converter::getRates() const
{
    const_pRates ps(cursy.size());
    auto it=ps.begin();
    for_each(cursy.begin(), cursy.end(), [&](const ExchangeRate& e)
    {
        *(it++)=&e;
    });
    return ps;
}

bool Converter::addRate(const ExchangeRate& e)
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
        //if (toLock)
         //   it->lock();
        *it=e;
        //if (toLock)
        //    it->unlock();
    }
    return true;
}

bool Converter::checkRate(QString bankName) const
{
    if (cursy.end()==find(bankName))
        return false;
    else
        return true;
}

ExchangeRate* Converter::getRate(QString bankName)
{
   auto it=find(bankName);
   return (cursy.end()==it) ? 0 : &(*it);
}

const ExchangeRate* Converter::getRate(QString bankName) const
{
    auto it=find(bankName);
    return (cursy.cend()==it) ? 0 : &(*it);
}
ExchangeRate* Converter::operator[] (QString bankName)
{
    auto it=find(bankName);
    return (cursy.end()==it) ? 0 : &(*it);
}

const ExchangeRate* Converter::operator[] (QString bankName) const
{
    auto it=find(bankName);
    return (cursy.cend()==it) ? 0 : &(*it);
}

double Converter::buyPrice(ValuteType valute, bool& isFinded,QString bankName) const
try{
    /*double returning=0;
    auto it=find(bankName);
    if (cursy.end()==it)
    {
        isFinded=false;
        return 0;
    }
    if (toLock)
        it->lock();
    isFinded=it->buyPrice(valute, returning);
    if (toLock)
        it->unlock();
    return returning;*/
    isFinded=true;
    return generalPrice(valute,buyType,bankName);
}
catch (ConvertationError) {
    isFinded=false;
    return 0;
}

double Converter::minBuyPrice(ValuteType valute, bool& isFinded,ValuteType baseValute, QString* bank) const
try{
   /*isFinded=false;
   auto it=min_element(cursy.cbegin(),cursy.cend(),[=](const ExchangeRate& e1, const ExchangeRate& e2)//->bool
   {
       double d1=0;
       double d2=0;
       bool finded1=e1.buyPrice(valute,d1),finded2=e2.buyPrice(valute,d2);
       return finded2 ? ((finded1 && d1<d2) ? true : false) : (finded1?true:false);
   });
    if (cursy.cend()==it)
    {
        isFinded=false;
        return 0;
    }
    if (toLock)
        it->lock();
    double returning=0;
    isFinded=it->buyPrice(valute, returning);
    if (toLock)
        it->unlock();
    return returning;*/
    isFinded=true;
    return extrenumPrice(valute, baseValute, buyType, minType, bank);
}
catch (ConvertationError) {
    isFinded=false;
    return 0;
}

double Converter::maxBuyPrice(ValuteType valute, bool& isFinded, ValuteType baseValute, QString* bank) const
try{
    /*isFinded=false;
    auto it=max_element(cursy.cbegin(),cursy.cend(), [=](const ExchangeRate& e1, const ExchangeRate& e2)//-> bool
    {
        double d1=0;
        double d2=0;
        bool finded1=e1.buyPrice(valute,d1),finded2=e2.buyPrice(valute,d2);
        return finded1 ? ((finded2 && d1<d2) ? true : false) : (finded2?true:false);
    });
     if (cursy.cend()==it)
     {
         isFinded=false;
         return 0;
     }
     if (toLock)
         it->lock();
     double returning=0;
     isFinded=it->buyPrice(valute, returning);
     if (toLock)
         it->unlock();
     return returning;*/
    isFinded=true;
    return extrenumPrice(valute, baseValute, buyType, maxType, bank);
}
catch (ConvertationError) {
    isFinded=false;
    return 0;
}

double Converter::sellPrice(ValuteType valute, bool& isFinded, QString bankName) const
try{
    /*auto it=find(bankName);
    if (cursy.end()==it)
    {
        isFinded=false;
        return 0;
    }
    if (toLock)
        it->lock();
    double returning=0;
    isFinded=it->sellPrice(valute, returning);
    if (toLock)
        it->unlock();
    return returning;*/
    isFinded=true;
    return generalPrice(valute,sellType,bankName);
}
catch (ConvertationError) {
    isFinded=false;
    return 0;
}

double Converter::minSellPrice(ValuteType valute, bool& isFinded, ValuteType baseValute, QString* bank) const
try{
    /*isFinded=false;
    auto it=min_element(cursy.cbegin(),cursy.cend(),[=](const ExchangeRate& e1, const ExchangeRate& e2)//->bool
    {
        double d1=0;
        double d2=0;
        bool finded1=e1.sellPrice(valute,d1),finded2=e2.sellPrice(valute,d2);
        return finded2 ? ((finded1 && d1<d2) ? true : false) : (finded1?true:false);
    });
     if (cursy.cend()==it)
     {
         isFinded=false;
         return 0;
     }
     if (toLock)
         it->lock();
     double returning=0;
     isFinded=it->sellPrice(valute, returning);
     if (toLock)
         it->unlock();
     return returning;*/
    isFinded=true;
    return extrenumPrice(valute, baseValute, sellType, minType, bank);
}
catch (ConvertationError) {
    isFinded=false;
    return 0;
}

double Converter::maxSellPrice(ValuteType valute, bool& isFinded, ValuteType baseValute, QString *bank) const
try{
    /*isFinded=false;
    auto it=max_element(cursy.cbegin(),cursy.cend(),[=](const ExchangeRate& e1, const ExchangeRate& e2)//->bool
    {
        double d1=0;
        double d2=0;
        bool finded1=e1.sellPrice(valute,d1),finded2=e2.sellPrice(valute,d2);
        return finded1 ? ((finded2 && d1<d2) ? true : false) : (finded2?true:false);
    });
     if (cursy.cend()==it)
     {
         isFinded=false;
         return 0;
     }
     if (toLock)
         it->lock();
     double returning=0;
     isFinded=it->sellPrice(valute, returning);
     if (toLock)
         it->unlock();
     return returning;*/
    isFinded=true;
    return extrenumPrice(valute, baseValute, sellType, maxType,bank);
}
catch (ConvertationError) {
    isFinded=false;
    return 0;
}

bool Converter::convert(Money from, Money& to, QString bankName) const
try{
    /*auto it=find(bankName);
    if (cursy.cend()==it)
        return false;
    if (toLock)
        it->lock();
    bool returning=it->convert(from,to);
    if (toLock)
        it->unlock();
    return returning;*/
    to.setValue(privateConvert(from,to.valute(),bankName));
    return true;
}
catch (ConvertationError)
{
    return false;
}

QStringList Converter::banks() const
{
    QStringList returning;
    for_each(cursy.begin(),cursy.end(), [&returning](const ExchangeRate& e)
    {
       returning<<e.bankName();
    });
    return returning;
}

bool  Converter::deleteRate(const QString& bankName,bool isNeeds)
{
    auto it=find(bankName);
    if (cursy.end()==it)
        return false;
    /*while(true)
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
    }*/
    cursy.erase(it);
    return true;
}

void Converter::createRates(const QStringList& sl)
{
    decltype(cursy) temp;
    for (auto it=sl.begin(); it!=sl.end(); ++it)
    {
        auto itTemp=find_if(cursy.begin(), cursy.end(), [&](const ExchangeRate& e)
        {
            return e.bankName()==*it;
        } );
        if (cursy.end()==itTemp)
            temp.push_back(ExchangeRate(*it));
        else
            temp.push_back(*itTemp);
    }
    clearRates();
    cursy=std::move(temp);
}

void Converter::clearRates(bool isNeeds)
{
    /*while(true)
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
    }*/
    cursy.clear();
}

Converter::ExchangeValutes Converter::valutes(const QString &bankName) const
{
    auto it=find(bankName);
    if (cursy.end()==it)
        return Converter::ExchangeValutes();
    //if (toLock)
    //    it->lock();
    Converter::ExchangeValutes returning=it->rateValutes();
    //if (toLock)
    //    it->unlock();
    return returning;
}

QStringList Converter::stringValutes(const QString& bankName) const
{
    auto it=find(bankName);
    if (cursy.end()==it)
        return QStringList();
    //if (toLock)
    //    it->lock();
    QStringList returning=it->stringValutes();
    //if (toLock)
    //    it->unlock();
    return returning;
}
