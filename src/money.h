#ifndef MONEY_H
#define MONEY_H

#include <string>
#include <utility>
#include <valutetype.h>
#include <stdexcept>
#include <QString>

//typedef std::pair<ValuteType,double> Money;

class MoneyOperationError: public std::runtime_error
{
private:
    QString happen;
public:

    explicit MoneyOperationError(QString s):runtime_error(""),happen(s)
    {}

    const char* what() const noexcept
    {
        return happen.toLatin1().data();
    }
};

class Money
{
public:
    Money(ValuteType valute=Hryvnia, double value=0):money(std::make_pair(valute,value))
    {}

    enum changeEnum
    {
        doConvert,
        stay
    };

    ValuteType valute() const
        { return money.first; }
    bool setValute(ValuteType type, changeEnum convertFlag=stay, QString bankName="")
    {
        bool b;
        switch (convertFlag)
        {
        case doConvert:
            *this=convert(type,b,bankName);
            return b;
            break;
        case stay:
            money.first=type;
            return true;
        }
        return false;
    }

    double value() const
        { return money.second; }
    void setValue(double value)
    { money.second=value; }

    Money convert(ValuteType type, bool& converted, QString bankName="") const;
    bool add(Money adding, QString bankName="")
        { return add(adding,valute(),bankName); }
    bool add(Money adding, ValuteType type, QString bankName="");
    bool sub(Money subbing, QString bankName="")
        { return sub(subbing,valute(),bankName); }
    bool sub(Money subbing, ValuteType type, QString bankName="");
    Money& operator+=(Money adding)
    {
        if (!add(adding))
            throw MoneyOperationError("bad adding with assignment");
        return *this;
    }
    Money& operator-=(Money subbing)
    {
        if (!sub(subbing))
            throw MoneyOperationError("bad subbing with assignment");
        return *this;
    }
    Money& operator/=(double divising2)
    {
        if (!divising2)
            throw MoneyOperationError("bad division: division by zero");
        money.second/=divising2;
        return *this;
    }
    Money& operator*=(double multing)
    {
        money.second*=multing;
        return *this;
    }

private:
    std::pair<ValuteType,double> money;
};

Money operator+(Money adding1,Money adding2);
Money operator-(Money subbing1,Money subbing2);
Money operator/(Money divising1,double divising2);
Money operator*(Money multing1,double multing2);
Money operator*(double multing1,Money multing2);

#endif // MONEY_H
