#include "money.h"
#include "converter.h"
using namespace std;

Money Money::convert(ValuteType type,bool& converted,QString bankName) const
{
    Money to(type);
    converted= Converter::instance()->convert(*this,to,bankName);
    return to;
}

bool Money::add(Money adding, ValuteType type,QString bankName)
{
    Money t(*this);
    if (!adding.setValute(type,doConvert,bankName) || !t.setValute(type,doConvert,bankName))
        return false;
    setValue(t.value()+adding.value());
    money.first=type;
    return true;
}

bool Money::sub(Money subbing, ValuteType type,QString bankName)
{
    Money t(*this);
    if (!subbing.setValute(type,doConvert,bankName) || !t.setValute(type,doConvert,bankName))
        return false;
    setValue(t.value()-subbing.value());
    money.first=type;
    return true;
}

Money operator+(Money adding1,Money adding2)
{

    if (!adding1.add(adding2))
        throw MoneyOperationError("bad add");
    return adding1;
}

Money operator-(Money subbing1,Money subbing2)
{
    if (!subbing1.sub(subbing2))
        throw MoneyOperationError("bad sub");
    return subbing1;
}

Money operator/(Money divising1,double divising2)
{
    return divising1/=divising2;
}

Money operator*(Money multing1,double multing2)
{
    return multing1*=multing2;
}

Money operator*(double multing1,Money multing2)
{
    return multing2*=multing1;
}
