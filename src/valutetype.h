#ifndef VALUTETYPE_H
#define VALUTETYPE_H
#include <QStringRef>
#include <QStringList>

enum ValuteTypeEnum
{
    Dollar,
    Hryvnia,
    Euro,
    Ruble
};

struct ValuteType
{
    ValuteTypeEnum type;

    ValuteType():type(Hryvnia)
        {}
    ValuteType(ValuteTypeEnum t):type(t)
        {}
    explicit ValuteType(const QStringRef& t)
    {
         QString s(t.toString());
         auto it=s.end();
         while ((--it)->isSpace())
         {
             s.remove(s.size()-1,1);
             it=s.end();
         }
         const QString UAH="UAH",USD="USD",EUR="EUR",RUR="RUR";
         if (UAH==s)
             type=Hryvnia;
         else if (USD==s)
             type=Dollar;
         else if (EUR==s)
             type=Euro;
         else if (RUR==s)
             type=Ruble;
         else
             type=static_cast<ValuteTypeEnum>(-1);
     }

    explicit ValuteType(QString s)
    {
        auto it=s.end();
        while ((--it)->isSpace())
        {
            s.remove(s.size()-1,1);
            it=s.end();
        }
        const QString UAH="UAH",USD="USD",EUR="EUR",RUR="RUR";
        if (UAH==s)
            type=Hryvnia;
        else if (USD==s)
            type=Dollar;
        else if (EUR==s)
            type=Euro;
        else if (RUR==s)
            type=Ruble;
        else
            type=static_cast<ValuteTypeEnum>(-1);
    }

    operator int() const
    {
        return type;
    }

    QString toQString() const
    {
        const QString UAH="UAH",USD="USD",EUR="EUR",RUR="RUR";
        if (Hryvnia==type)
            return UAH;
        else if (type==Dollar)
            return USD;
        else if (type==Euro)
            return EUR;
        else if (type==Ruble)
            return RUR;
        else
            return QString();
    }
    static QStringList valutes()
    {
        const QString UAH="UAH",USD="USD",EUR="EUR",RUR="RUR";
        QStringList returning;
        returning<<UAH<<USD<<EUR<<RUR;
        return returning;
    }
};

inline bool operator==(ValuteType t1,ValuteType t2)
{
    return t1.type==t2.type;
}

inline bool operator!=(ValuteType t1,ValuteType t2)
{
    return !(t1==t2);
}

#endif // VALUTETYPE_H
