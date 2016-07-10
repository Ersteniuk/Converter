#include "converterrate.h"
#include "algorithm"
using namespace std;

double ConverterRate::convertBase(Money from, ValuteType to, ConverterRate::ConvertWayLong how,
                              int maxListLong,bool isCircleError) const
{
    ConvertList clTo=move(toBase(to,maxListLong,isCircleError,how));
    ConvertList clFrom=move(toBase(from.valute(),maxListLong,isCircleError,how));
    switch (how)
    {
    case minWay:
    {
        auto p=findMinPath(move(clFrom),move(clTo));
        clFrom=move(p.first);
        clTo=move(p.second);
        break;
    }
    case fullWay:
    {
        if (clTo.back()!=base || clFrom.back()!=base)
            throw ConverterRateError(ConverterRateError::tooLongWay);
        break;
    }
    default:
        throw ConverterRateError(ConverterRateError::unknownConvertWayLong);
    }
    return moveSell( moveBuy(from.value(),clFrom) , clTo );
}

ConverterRate::ConvertList ConverterRate::toBase(ValuteType type, int maxListLong, bool isCircleError,
                                                 ConvertWayLong how) const
{
    ConvertList temp;
    while ( type!=base && temp.size()<maxListLong)
    {
        if (checkCircle(temp,type,isCircleError))
            return temp;
        temp.push_back(type);
        try{
            type=findValute(type).base;
        }
        catch (ConverterRateError er)
        {
            if (minWay==how && ConverterRateError::unknownValute==er.error)
                return temp;
            else
                throw;
        }
    }
    if (checkCircle(temp,type,isCircleError) || temp.size()==maxListLong)
        return temp;
    temp.push_back(type);
    return temp;
}

std::pair<ConverterRate::ConvertList,ConverterRate::ConvertList> ConverterRate::findMinPath
    (ConverterRate::ConvertList&& one,ConverterRate::ConvertList&& two) const
{
    if (one.size()==0 || two.size()==0)
            throw ConverterRateError(ConverterRateError::unrealWay);
    auto p=make_pair(move(one),move(two));
    int maxLength=p.first.size()+p.second.size()-1;//first we should check all, but then one we exclude
    auto it1=p.first.end(),it2=p.second.end();
    for(int first=0; first<p.first.size(); ++first)
    {
        int maxValue=maxLength-first<static_cast<long long>(p.second.size()) ? maxLength-first : p.second.size();
        for (int second=0; second<maxValue; ++second)
            if (p.first[first]==p.second[second])
            {
                it1=p.first.begin()+first;
                it2=p.second.begin()+second;
                maxLength=first+second;
                break;
            }
    }
    if (p.first.end()==it1 || p.second.end()==it2)
        throw ConverterRateError(ConverterRateError::unrealWay);

    p.first.erase(it1+1 , p.first.end() );
    p.second.erase(it2+1 , p.second.end() );
    return p;
}

double ConverterRate::moveBuy(double value,const ConverterRate::ConvertList& e) const
{
    auto end=e.end();
    --end;
    for (auto it=e.begin();it!=end;++it)
        value*=findValute(*it).buy;
    return value;
}

double ConverterRate::moveSell(double value,const ConverterRate::ConvertList& e) const
{
    auto rb=e.rbegin();
    for (++rb;rb!=e.rend();++rb)
        value/=findValute(*rb).sell;
    return value;
}

ConverterRate::ExchangeValute ConverterRate::findValute(ValuteType type) const
{
    auto it=find_if(curses.begin(),curses.end(), [type](const ExchangeValute& e)
    {
        return e.type==type;
    });
    if (it==curses.end())
        throw ConverterRateError(ConverterRateError::unknownValute);
    return *it;
}

bool ConverterRate::checkCircle(const ConverterRate::ConvertList& cl, ValuteType type, bool isCircleError) const
{
    auto it=find_if(cl.begin(),cl.end(), [type](const ValuteType& vt)
    {
        return vt==type;
    });
    if (it!=cl.end())
    {
        if (isCircleError)
            throw ConverterRateError(ConverterRateError::circleWay);
        else
            return true;
    }
    return false;
}
