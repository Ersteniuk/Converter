#include "extractor.h"
#include <QRegExp>
#include <QStringList>
#include <algorithm>
#include <utility>
#include <QMessageBox>
#include <QTextStream>

#include "errormanagement.h"
using namespace std;

AbstractExtractor::AbstractExtractor(const QString& fileName):ifs(fileName)
{
    ifs.open(QIODevice::ReadOnly);
    ErrorManagement::checkFile(ifs,true);
    //checkFile(true);
}

AbstractExtractor::Rates AbstractExtractor::extract(const QString& s)
{
    Rates returning;
    preWork(returning,s);
    ExchangeValute val;
    while (read())
    try {
        val=extractOne(s);
        returning.push_back(val);
    }
    catch (const UpdateError& e) {
        if (!UpdateError::small==e.errorType)
            throw;
    }
    return returning;
}

/*ValuteType AbstractBaseExtractor::type(std::string s)
{
    const string UAH="UAH",USD="USD",EUR="EUR",RUR="RUR";
    if (UAH==s)
        return Hryvnia;
    if (USD==s)
        return Dollar;
    if (EUR==s)
        return Euro;
    if (RUR==s)
        return Ruble;
    return static_cast<ValuteType>(-1);
}*/

qint64 RegexExtractor::readBuySellLine(const QString& type,bool shouldBeLine, QString& s)
{
    bool ok;
    quint64 number;
    const QChar endBracket=']',equal='=';
    char ch;
    QString line,lStr,numb;
    ErrorManagement::FileChecker fCheck(ifs,"Unexpected character in the file");
    line=ifs.readLine(type.size()+1);
    fCheck.check();
    if (type!=line)
        fCheck.error();
    while (ifs.getChar(&ch) && QChar(ch).isDigit())
        numb+=ch;
    fCheck.check();
    number=numb.toInt(&ok);   
    if (!ok)
        fCheck.error();
    if (endBracket!=ch)
         fCheck.error();

    ifs.getChar(&ch);
    fCheck.check();

    if (shouldBeLine)
    {
        if (equal!=ch)
            fCheck.error();
        else
        {
            lStr=ifs.readLine();
            deleteWhitespaceInEnd(lStr);
        }
    }
    else
    {
        if (equal==ch)
        {
            lStr=ifs.readLine();
            deleteWhitespaceInEnd(lStr);
        }
    }
    s=lStr;
    return number;
}

bool RegexExtractor::read()
{
    const QString base="base=";
    const QString splitter="splitter=",buy="buy[",sell="sell[";
    const QChar endPart='}',beginPart='{';
    char ch=0;
    bool ok;
    QString line,lStrBuy,lStrSell;
    qint64 lNBuy,lNSell;
    char lSplitter;
    ValuteType lType,lBase;
    ErrorManagement::FileChecker fCheck(ifs,"Unexpected character in the file");

    do
    {
        ifs.getChar(&ch);
        if (ifs.atEnd())
            return false;
        fCheck.check();
    }
    while (QChar(ch).isSpace());
    if (beginPart!=ch)
    {
        ifs.ungetChar(ch);
        fCheck.error();

    }
    ifs.getChar(nullptr);//skip the \n-char
    line=ifs.readLine();
    fCheck.check();

    lType=ValuteType(line);
    if (-1==lType)
        fCheck.error("Unknown valute type in the file");
    line=ifs.readLine(base.size()+1);
    if (base==line)
    {
        line=ifs.readLine();
        fCheck.check();
        lBase=ValuteType(line);
        line=ifs.readLine(splitter.size()+1);
    }
    else
    {
        lBase=baseType;
        line+=ifs.readLine(splitter.size()+1-base.size());
    }

    fCheck.check();
    if (splitter!=line)
        fCheck.error();
    ifs.getChar(&lSplitter);
    ifs.readLine();//skip the \n-char

    lNBuy=readBuySellLine(buy,true,lStrBuy);
    lNSell=readBuySellLine(sell,false,lStrSell);
    ifs.getChar(&ch);
    fCheck.check();
    if (endPart!=ch)
        fCheck.error();
    pars={lType,lBase,lSplitter,lNBuy,lStrBuy,lNSell,lStrSell};
    return true;
}

ExchangeRate::ExchangeValute RegexExtractor::extractOne(const QString& s)
{
    bool ok;
    const QChar dot='.';
    ExchangeValute val;
    ErrorManagement::Checker cheker("Unexpected character in server information");
    QRegExp reg(pars.strBuy);
    reg.indexIn(s);
    QString qs=reg.capturedTexts()[pars.nBuy];
    qs.replace(pars.splitter,dot);
    double buyCost=qs.toDouble(&ok);
    if (!ok)
        cheker.error();
    if (!pars.strSell.isEmpty())
    {
        reg=QRegExp (pars.strSell);
        reg.indexIn(s);
    }
    qs=reg.capturedTexts()[pars.nSell];
    qs.replace(pars.splitter,dot);
    double sellCost=qs.toDouble(&ok);
    if (!ok)
        cheker.error();
    return val={pars.type,sellCost,buyCost,pars.base};
}

/*bool PrivatApiExtractor::readAttribute(int i,QString& cast)
{
    const QString attribut="attribute[";
    const QStringList consts={"type","base","buy","sell"};
    const QChar endBracket=']',equal='=';
    QString line1,line2;
    char ch1,ch2;
    ErrorManagement::FileChecker fCheck(ifs,"Unexpected character in the file");
    line1=ifs.readLine(attribut.size()+1);
    line2=ifs.readLine(consts[i].size()+1);
    ifs.getChar(&ch1);
    ifs.getChar(&ch2);
    fCheck.check();
    //if (!AbstractExtractor::staticCheckFile(is,true))
     //   return false;
    if (attribut!=line1 || consts[i]!=line2 || endBracket!=ch1 || equal!=ch2)
        fCheck.error();
    cast=ifs.readLine();
    AbstractBaseExtractor::deleteWhiteSpaceInEnd(cast);
    return true;
}*/

bool PrivatApiExtractor::readInfoPair(ParseStruct::InfoPair& p)
{
    //ParseStruct::InfoPair ip;
    ValuteType lType;
    QString line;
    char ch1,ch2;
    const int numberInValute=3+1;
    const QChar endPart='}',beginPart='{',equal='=';
    ErrorManagement::FileChecker fCheck(ifs,"Unexpected character in the file");
    do
    {
        ifs.getChar(&ch1);
        //if (!AbstractExtractor::staticCheckFile(is,true))
        //    return false;
        fCheck.check();
    }
    while (QChar(ch1).isSpace());
    if (beginPart!=ch1)
    {
        ifs.ungetChar(ch1);
        return false;
    }
    ifs.readLine();//skip the \n char
    line=ifs.readLine(numberInValute);
    fCheck.check();
   // if (!AbstractExtractor::staticCheckFile(is,true))
     //   return false;
    lType=ValuteType(line);
    if (-1==lType)
        fCheck.error();
    /*{
        QMessageBox::warning(0,is.fileName(),"Unknown valute type in the file");
        return false;
    }*/
    //ip.second=lType;
    ifs.getChar(&ch1);
    line=ifs.readLine();
    //AbstractBaseExtractor::deleteWhiteSpaceInEnd(line);
    deleteWhitespaceInEnd(line);
    ifs.getChar(&ch2);
    fCheck.check();
   // if (!AbstractExtractor::staticCheckFile(is,true))
    //    return false;
    if (equal!=ch1 || endPart!=ch2)
        fCheck.error("Unexpected character in the file");
    /*
        QMessageBox::warning(0,is.fileName(),"Unexpected character in the file");
        return false;
    }*/
    //p.first=line;
    p={line,lType};
    return true;
}

PrivatApiExtractor::ParticularInfo PrivatApiExtractor::readParticular(ParticularInfo::InfoKind kind)
{
    ParticularInfo inf;
    inf.kind=kind;
    inf.wasRead=false;
    const QChar beginBracket='[',endBracket=']',equal='=',comma=',';
    const char true_='1', false_='0';
    QString line1,line2;
    char ch1,ch2;
    ErrorManagement::FileChecker fCheck(ifs,"Unexpected character in the file",UpdateError::huge);
    ifs.getChar(&ch1);
    ifs.getChar(&ch2);
    fCheck.check();
    if (beginBracket!=ch1)
        fCheck.error();
    switch (ch2) {
    case true_:
    {
        inf.isInAttribute=true;
        ifs.getChar(&ch2);
        while (ifs.getChar(&ch1) && QChar(ch1)!=endBracket)
            line2.append(ch1);
        fCheck.check();
        if (comma!=ch2)
            fCheck.error();
        inf.attribute=line2;
        break;
    }
    case false_:
        inf.isInAttribute=false;
        ifs.getChar(&ch1);
        break;
    default:
        fCheck.error();
    }
    ifs.getChar(&ch2);
    line1=ifs.readLine();
    fCheck.check();
    if (endBracket!=ch1 || equal!=ch2)
        fCheck.error();
    deleteWhitespaceInEnd(line1);
    inf.tagName=line1;
    return inf;
}

bool PrivatApiExtractor::readParse()
{
    ParseStruct::InfoPairs lTypes;
    ParseStruct::InfoPair p;
    QString line;
    const QStringList infoKind({"type","base","buy","sell"});
    const QVector<ParticularInfo::InfoKind> infoKinds({ParticularInfo::mainType,ParticularInfo::baseType,
                                                       ParticularInfo::buyType,ParticularInfo::sellType});
    const QChar endPart='}',beginPart='{';//,equal='=';
    char ch;

    ErrorManagement::FileChecker fCheck(ifs,"Unexpected character in the file",UpdateError::huge);
    QVector<bool> useds(ParseStruct::n,false);

    ifs.getChar(&ch);
    if (ifs.atEnd())
        return false;
    fCheck.check();
    if (beginPart!=ch)
    {
        ifs.ungetChar(ch);
        fCheck.error();
    }
    ifs.getChar(nullptr);//skip the \n-char
    for (int i=0; i<ParseStruct::n; ++i)
    {
        line=QString();
        while (ifs.getChar(&ch) && QChar(ch).isLetter() )
            line+=ch;
        fCheck.check();
        ifs.ungetChar(ch);
        bool used=false;
        for (int i=0; i<ParseStruct::n; ++i)
            if (infoKind[i]==line)
            {
                pars.pInfos[i]=readParticular(infoKinds[i]);
                useds[i]=true;
                used=true;
                break;
            }
        if (!used)
            fCheck.error();
    }
    for (int i=0; i<ParseStruct::n; ++i)
        if (!useds[i])
            fCheck.error("Unfull information");
    /*ifs.getChar(&ch);
    fCheck.check();
    if (name!=line || equal!=ch)
        fCheck.error();
    lName=ifs.readLine();
    AbstractBaseExtractor::deleteWhiteSpaceInEnd(lName);
    for (int i=0; i<n; ++i)
        if (!readAttribute(i,line))
            return false;
        else
            strings<<line;*/
    while (readInfoPair(p))
        lTypes.push_back(p);
    ifs.getChar(&ch);
    fCheck.check();
    if (endPart!=ch)
        fCheck.error();
    pars.types=lTypes;
    return true;
}

PrivatApiExtractor::ParseStruct::pIterator PrivatApiExtractor::ParseStruct::find(const QString& s)
{
    auto it=find_if(pInfos.begin(), pInfos.end(), [&](ParticularInfo& p)
    {
        return (s==p.tagName);
    });
    return it;
}

PrivatApiExtractor::ParseStruct::pIterator PrivatApiExtractor::ParseStruct::find(const QString& s, pIterator pIt)
{
    auto it=find_if(pIt, pInfos.end(), [&](ParticularInfo& p)
    {
        return (s==p.tagName);
    });
    return it;
}

ValuteType PrivatApiExtractor::find(const QString& s)
{
    auto it=find_if(pars.types.begin(), pars.types.end(), [&](ParseStruct::InfoPair& p)
    {
        return (s==p.first);
    });
    if (it==pars.types.end())
        return static_cast<ValuteTypeEnum>(-1);
    return it->second;
}

QString PrivatApiExtractor::find(ValuteType type)
{
    auto it=find_if(pars.types.begin(), pars.types.end(), [&](ParseStruct::InfoPair& p)
    {
        return (type==p.second);
    });
    return it->first;
}

bool PrivatApiExtractor::read()
{
    while (!reader.atEnd())
    {
        reader.readNext();
        // qDebug() <<reader.lineNumber()<<':'<<reader.columnNumber()<<endl;
        if (reader.tokenType()==QXmlStreamReader::StartElement)
            for (auto it=pars.pInfos.begin(); it!=pars.pInfos.end(); ++it)
                if (reader.name()==it->tagName)
                    return true;
    }
    return false;
}

ExchangeRate::ExchangeValute PrivatApiExtractor::extractOne(const QString& s)
{
    bool ok;
    QString str;
    ExchangeValute val;
    ErrorManagement::Checker cheker("Fail information in settings' file");
    pars.zeroedAll();
    while (true)
    {
        auto it=pars.find(reader.name().toString());
        while (it!=pars.pInfos.end())
        {
            if (!it->isInAttribute)
            {
                if (reader.readNext()!=QXmlStreamReader::Characters)
                    cheker.error();
                str=reader.text().toString();
            }
            else
            {
                auto at=reader.attributes();
                str=at.value(it->attribute).toString();
            }

            switch (it->kind)
            {
            case ParticularInfo::mainType:
            {
                auto type=find(str);
                if (static_cast<int>(type)==-1)
                    cheker.error("Unknown valute",UpdateError::small);
                val.type=type;
                if (it->wasRead)
                    cheker.error();
                it->wasRead=true;
                break;
            }
            case ParticularInfo::baseType:
            {
                auto type=find(str);
                if (static_cast<int>(type)==-1)
                    cheker.error("Unknown valute",UpdateError::small);
                val.base=type;
                if (it->wasRead)
                    cheker.error();
                it->wasRead=true;
                break;
            }
            case ParticularInfo::sellType:
            {
                val.sell=str.toDouble(&ok);
                if (!ok)
                    cheker.error("Unexpected character in server information");
                if (it->wasRead)
                    cheker.error();
                it->wasRead=true;
                break;
            }
            case ParticularInfo::buyType:
            {
                val.buy=str.toDouble(&ok);
                if (!ok)
                    cheker.error("Unexpected character in server information");
                if (it->wasRead)
                    cheker.error();
                it->wasRead=true;
                break;
            }
            default:
                cheker.error("Bad type");
            }

            ++it;
            it=pars.find(reader.name().toString(),it);
        }
        if (pars.checkAll())
            return val;
        do
        {
            reader.readNext();
            if (reader.atEnd())
                cheker.error();
        }
        while ( reader.tokenType()!=QXmlStreamReader::StartElement ||
                pars.find(reader.name().toString())!=pars.pInfos.end() );
    }
}
