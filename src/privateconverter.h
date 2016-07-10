#ifndef PRIVATECONVERTER_H
#define PRIVATECONVERTER_H
#include <list>
#include "exchangerate.h"
#include "errormanagement.h"

class ConvertationError
{
public:
    enum Reason{ unknownBank, inBankError, unknownOperationType, unknownExtrenumType } reason;

    ConvertationError(Reason r):reason(r)
        {}
};

class PrivateConverter
{
protected:
    typedef std::list<ExchangeRate>::iterator iterator;
    typedef std::list<ExchangeRate>::const_iterator const_iterator;
    typedef std::list<ExchangeRate> Rates;

    enum operationType {buyType,sellType};
    enum extrenumType {minType,maxType};

    Rates cursy;
public:
    virtual ~PrivateConverter()
    {}

    virtual iterator find(QString bankName);
    virtual const_iterator find(QString bankName) const;

    virtual double privateConvert(Money from, ValuteType to, QString bankName) const;
    virtual double generalPrice(ValuteType valute, operationType t, QString bankName) const;
    virtual double extrenumPrice(ValuteType valute,ValuteType baseValute, operationType t, extrenumType e,
                                 QString* bankName) const;
};

#endif // PRIVATECONVERTER_H
