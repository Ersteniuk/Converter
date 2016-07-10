#ifndef EXTRACTOR_H
#define EXTRACTOR_H
//#include <fstream>
//#include <string>
#include <QFile>
#include <vector>
#include "exchangerate.h"
#include "valutetype.h"
#include <QXmlStreamReader>


#include <QDebug>
class QNetworkReply;

class AbstractExtractor
{
protected:
    typedef ExchangeRate::ExchangeValute ExchangeValute;
    typedef std::vector<ExchangeValute> Rates;

    //std::ifstream ifs;
    QFile ifs;

    virtual bool read()=0;
    virtual void preWork(Rates&, const QString&)=0;
    virtual ExchangeValute extractOne(const QString& s)=0;
public:
    AbstractExtractor(const QString& fileName);
    virtual ~AbstractExtractor()
    {
        ifs.close();
    }

    Rates extract(const QString& s);
    Rates operator() (const QString& s)
        { return extract(s); }
};

class AbstractBaseExtractor:public AbstractExtractor
{
protected:
    ValuteType baseType;
    void preWork(Rates& r,const QString&)
        {}
    void deleteWhitespaceInEnd(QString& s)
    {
        auto it=s.end();
        while ((--it)->isSpace())
        {
            s.remove(s.size()-1,1);
            it=s.end();
        }
    }
    /*static void deleteWhiteSpaceInEnd(QString& s)
    {
        auto it=s.end();
        while ((--it)->isSpace())
        {
            s.remove(s.size()-1,1);
            it=s.end();
        }
    }*/

public:
   AbstractBaseExtractor(const QString& fileName, ValuteType base):
       AbstractExtractor(fileName),baseType(base)
        {}
   ~AbstractBaseExtractor()
            {}
};


class RegexExtractor: public AbstractBaseExtractor
{
private:
    struct ParseStruct
    {
        ValuteType type;
        ValuteType base;
        char splitter;
        int nBuy;
        QString strBuy;
        int nSell;
        QString strSell;
    };

    ParseStruct pars;

    qint64 readBuySellLine(const QString& type, bool shouldBeLine, QString& s);
protected:
    bool read();
    ExchangeValute extractOne(const QString& s);
public:
    RegexExtractor(const QString& fileName, ValuteType base):AbstractBaseExtractor(fileName,base)
        {}

   static Rates work(const QString& s,const QString& fileName, ValuteType base=Hryvnia)
   {
       RegexExtractor ex(fileName,base);
       return ex.extract(s);
   }
};

class PrivatApiExtractor: public AbstractBaseExtractor
{
private:
    struct ParticularInfo
    {
        enum InfoKind { mainType, baseType, sellType, buyType } kind;
        QString tagName;
        bool isInAttribute;
        QString attribute;
        bool wasRead;
    };

    struct ParseStruct
    {
        typedef std::pair<QString,ValuteType> InfoPair;
        typedef std::vector<InfoPair> InfoPairs;
        typedef InfoPairs::iterator iterator;
        typedef InfoPairs::const_iterator const_interator;
        typedef std::vector<ParticularInfo> ParticularInfos;
        typedef ParticularInfos::iterator pIterator;
        typedef ParticularInfos::const_iterator pConst_Iterator;

        InfoPairs types;
        static const int n=4;
        std::vector<ParticularInfo> pInfos;

        pIterator find(const QString &s);
        pIterator find(const QString &s, pIterator pIt);
        void zeroedAll()
        {
            std::for_each(pInfos.begin(), pInfos.end(), [](ParticularInfo& p)
            {
                p.wasRead=false;
            });
        }
        bool checkAll() const
        {
            if (pInfos.end()==std::find_if(pInfos.begin(), pInfos.end(), [](const ParticularInfo& p)
            {
                                           return !(p.wasRead);
            }) )
                return true;
            else
                return false;
        }
    };

    ParseStruct pars;
    QXmlStreamReader reader;

    ValuteType find(const QString& s);
    QString find(ValuteType type);
    ParticularInfo readParticular(ParticularInfo::InfoKind kind);
    bool readParse();

    //bool readAttribute(int i, QString& cast);
    bool readInfoPair(ParseStruct::InfoPair& p);
protected:
    bool read();

    void preWork(Rates& r, const QString& s)
    {
        reader.addData(s);
        AbstractBaseExtractor::preWork(r,s);
    }

    ExchangeValute extractOne(const QString& s);
public:
    PrivatApiExtractor(const QString& fileName, ValuteType base):
        AbstractBaseExtractor(fileName,base)
        {
            pars.pInfos.resize(ParseStruct::n);
            readParse();
        }

    ~PrivatApiExtractor()
    {
        auto dev=reader.device();
        if (dev)
           dev->deleteLater();
    }

    static Rates work(const QString& s,const QString& fileName, ValuteType base=Hryvnia)
    {
        PrivatApiExtractor ex(fileName,base);
        return ex.extract(s);
    }
};

#endif // EXTRACTOR_H
