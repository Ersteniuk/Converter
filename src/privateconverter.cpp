#include "privateconverter.h"
#include "functional"

PrivateConverter::iterator PrivateConverter::find(QString bankName)
{
    return cursy.empty() ? cursy.end() : find_if(cursy.begin(), cursy.end(), [=](const ExchangeRate& e)->bool
                    {
                        return e.bankName()==bankName;
                    } );
}

PrivateConverter::const_iterator PrivateConverter::find(QString bankName) const
{
    return cursy.empty() ? cursy.cend() : find_if(cursy.cbegin(), cursy.cend(), [=](const ExchangeRate& e)
                    {
                        return e.bankName()==bankName;
                    } );
}

double PrivateConverter::privateConvert(Money from, ValuteType to, QString bankName) const
{
    auto it=find(bankName);
    if (cursy.cend()==it)
        throw ConvertationError(ConvertationError::unknownBank);
    Money mto(to);
    if (!it->convert(from,mto))
        throw ConvertationError(ConvertationError::inBankError);
    return mto.value();
}

double PrivateConverter::generalPrice(ValuteType valute, operationType t, QString bankName) const
{
    double returning=0;
    auto it=find(bankName);
    if (cursy.end()==it)
        throw ConvertationError(ConvertationError::unknownBank);
    auto somePrice=&ExchangeRate::buyPrice;
    switch (t)
    {
    case buyType:
        break;
    case sellType:
        somePrice=&ExchangeRate::sellPrice;
        break;
    default:
        throw ConvertationError(ConvertationError::unknownOperationType);
    }
    if (! ((*it).*somePrice)(valute, returning))
        throw ConvertationError(ConvertationError::inBankError);
    return returning;
}

double PrivateConverter::extrenumPrice(ValuteType valute, ValuteType baseValute, operationType t, extrenumType e,
                                       QString *bankName) const
{
    if (t!=sellType && t!=buyType)
        throw ConvertationError(ConvertationError::unknownOperationType);
    auto buySearch=[=](const ExchangeRate& e1, const ExchangeRate& e2)//->bool
    {
        Money m1(baseValute,1);
        Money m2(baseValute,1);
        bool finded1=e1.convert(Money(valute,1),m1),finded2=e2.convert(Money(valute,1),m2);
        return finded2 ? (finded1 ? ( m1.value()<m2.value() ? true : false): (maxType==e ?  true : false) ):
                         (finded1 ?(maxType==e ? false : true ) : false);
    };
    auto sellSearch=[=](const ExchangeRate& e1, const ExchangeRate& e2)//->bool
            {
                Money m1(valute,1);
                Money m2(valute,1);
                bool finded1=e1.convert(Money(baseValute,1),m1),finded2=e2.convert(Money(baseValute,1),m2);
                return finded2 ? (finded1 ? ( 1/m1.value()<1/m2.value() ? true : false): (maxType==e ?  true : false) ):
                                 (finded1 ?(maxType==e ? false : true ) : false);
            };
    const_iterator it;
    switch (e)
    {
    case minType:
        t==sellType ? (it=min_element(cursy.cbegin(),cursy.cend(),sellSearch)) :
                      (it=min_element(cursy.cbegin(),cursy.cend(),buySearch));
        break;
    case maxType:
        t==sellType ? (it=max_element(cursy.cbegin(),cursy.cend(),sellSearch)) :
                      (it=max_element(cursy.cbegin(),cursy.cend(),buySearch));
        break;
    default:
        throw ConvertationError(ConvertationError::unknownExtrenumType);
    }
    if (cursy.cend()==it)
        throw ConvertationError(ConvertationError::inBankError);

    Money b(baseValute,1);
    Money v(valute,1);
    bool ok;
    double returning=0;

    switch (t)
    {
    case buyType:
        ok=it->convert(v,b);
        returning=b.value();
        break;
    case sellType:
        ok=it->convert(b,v);
        returning=1/v.value();
        break;
    default:
        throw ConvertationError(ConvertationError::unknownOperationType);
    }

    if (!ok)
        throw ConvertationError(ConvertationError::inBankError);
    if (bankName)
        *bankName=it->bankName();
    return returning;
}
