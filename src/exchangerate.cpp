#include "exchangerate.h"
#include <algorithm>
#include <QStringList>
using namespace std;

QMutex ExchangeRate::mutex;

ExchangeRate::ExchangeRate(QString bankName, const vector<ExchangeRate::ExchangeValute>& cs, ValuteType base_):
    ConverterRate(cs,base_),bank(bankName),lastUpdate_(QDateTime::currentDateTime())
        {}

ExchangeRate::ExchangeRate(QString bankName,ValuteType base_):
    ConverterRate(RateVector(),base_),bank(bankName),lastUpdate_(QDateTime::currentDateTime())
        {}

ExchangeRate::ExchangeRate():ConverterRate(RateVector(),Hryvnia),lastUpdate_(QDateTime::currentDateTime())
        {}

/*ExchangeRate::ExchangeRate(const ExchangeRate& e):ConverterRate(e.curses,e.base),bank(e.bankName()),
    lastUpdate_(e.lastUpdate())
        {}*/

/*ExchangeRate& ExchangeRate::operator=(const ExchangeRate& e)
{
    bank=e.bankName();
    curses=e.curses;
    lastUpdate_=e.lastUpdate();
    base=e.bankBase();
    return *this;
}*/

bool ExchangeRate::setRate(const std::vector<ExchangeRate::ExchangeValute>& cs)
{
    if (find_if(cs.begin(), cs.end(),[](ExchangeRate::ExchangeValute v)
        {
                            return v.buy<=0 || v.sell<=0;
        } )!=cs.end())
        return false;
    curses=cs;
    lastUpdate_=QDateTime::currentDateTime();
    return true;
}

std::vector<ValuteType> ExchangeRate::valutes() const
{
    std::vector<ValuteType> returning;
    returning.push_back(base);
    for_each(curses.begin(), curses.end(), [&returning] (const ExchangeValute& e)
    {
        returning.push_back(e.type);
    });
    return returning;
}

ExchangeRate::RateVector ExchangeRate::rateValutes() const
{
    return curses;
}

QStringList ExchangeRate::stringValutes() const
{
    QStringList returning;
    returning<<base.toQString();
    for_each(curses.begin(), curses.end(), [&returning] (const ExchangeValute& e)
    {
        returning<<e.type.toQString();
    });
    return returning;
}

bool ExchangeRate::convert(Money from, Money& to) const
{
    /*bool is_find;
    if (from.valute()==to.valute())
    {
        to.setValue(from.value());
        return true;
    }
    double buyPrice_=buyPrice(from.valute(), is_find);
    if (!is_find)
        return false;
    double sellPrice_=sellPrice(to.valute(), is_find);
    if (!is_find)
        return false;
    to.setValue(from.value()*buyPrice_/sellPrice_);
    return true;*/
    ConvertWayLong how;
    unsigned int n;
    bool isCircleError;
    setting(how,n,isCircleError);
    try
    {
        to.setValue(convertBase(from,to.valute(),how,n,isCircleError));
    }
    catch (ConverterRateError er)
    {
        return errorHandling(er);
    }
    return true;
}

bool ExchangeRate::buyPrice(ValuteType what,double& result) const
{
    /*auto it=find_if(curses.begin(), curses.end(), [what](ExchangeRate::ExchangeValute v1)
    { return v1.type==what?true:false; } );
    if (curses.end()==it)
    {
        isInBank=false;
        return 0;
    }
    else
    {
        isInBank=true;
        return it->buy;
    }*/
    Money ret(base);
    bool b=convert(Money(what,1),ret);
    if (b)
        result=ret.value();
    return b;
}

bool ExchangeRate::sellPrice(ValuteType what,double& result) const
{
    /*
    auto it=find_if(curses.begin(), curses.end(),[what](ExchangeRate::ExchangeValute v1)
        { return v1.type==what?true:false; } );
    if (curses.end()==it)
    {
        isInBank=false;
        return 0;
    }
    else
    {
        isInBank=true;
        return it->sell;
    }*/
    Money ret(what);
    bool b=convert(Money(base,1),ret);
    if (b)
        result=1/ret.value();
    return b;
}

ExchangeRate::ExchangeValute ExchangeRate::price(ValuteType what,bool& isInBank) const
{
    auto it=find_if(curses.begin(), curses.end(),[what](ExchangeRate::ExchangeValute v1)
        { return v1.type==what?true:false; } );
    if (curses.end()==it)
    {
        isInBank=false;
        return ExchangeValute();
    }
    else
    {
        isInBank=true;
        return *it;
    }
}

void ExchangeRate::setting(ConvertWayLong& how,unsigned int& n,bool& isCircleError) const
{
    how=fullWay;
    n=5;
    isCircleError=true;
}

bool ExchangeRate::errorHandling(ConverterRateError er) const
{
    return false;
}
