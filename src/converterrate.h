#ifndef CONVERTERRATE_H
#define CONVERTERRATE_H
#include "valutetype.h"
#include "money.h"
#include <vector>

struct ConverterRateError
{
    enum ConverterRateErrorEnum { tooLongWay,circleWay,unrealWay,unknownValute,unrealRate,unknownConvertWayLong } error;
    ConverterRateError(ConverterRateErrorEnum e):error(e)
        {}
};

struct ExchangeValue
{
    ValuteType type;
    double sell;//value on how bank sell valute
    double buy;//value on how bank buy valute
    ValuteType base;
};

class ConverterRate
{
public:
    enum ConvertWayLong { fullWay, minWay };

    typedef ExchangeValue ExchangeValute;
    typedef std::vector<ExchangeValute> RateVector;
    typedef std::vector<ValuteType> ConvertList;
protected:
    std::vector<ExchangeValue> curses;
    ValuteType base;
public:
    ConverterRate(const RateVector& curses_, ValuteType base_):curses(curses_),base(base_)
        {}


    double convertBase(Money from, ValuteType to, ConvertWayLong how, int maxListLong,bool isCircleError) const;
    ConvertList toBase(ValuteType type, int maxListLong, bool isCircleError, ConvertWayLong how=fullWay) const;
    std::pair<ConvertList,ConvertList> findMinPath(ConvertList &&one, ConvertList &&two) const;
    virtual double moveBuy(double value,const ConvertList& e) const;
    virtual double moveSell(double value,const ConvertList& e) const;
    ExchangeValute findValute(ValuteType type) const;
    bool checkCircle(const ConvertList& cl,ValuteType type,bool isCircleError) const;
};

#endif // CONVERTERRATE_H
