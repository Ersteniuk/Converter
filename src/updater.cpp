#include "defening.h"
#include "updater.h"
#include <algorithm>
#include "exchangerate.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>
#include <functional>
#include "extractor.h"
#include <iostream>
#include "extractor.h"
#include <memory>
#include "fstream"
#include <QMessageBox>
#include <QFile>

#include "updateerror.h"
#include "errormanagement.h"
using namespace std;

Updater::Updater(QString infoFile,int32_t timeInterval,ValuteType baseType,QObject *parent):
    AbstractUpdater(timeInterval,parent),loader(nullptr),base(baseType)
{
    QFile f(infoFile);
    ErrorManagement::FileChecker fCheck(f,UpdateError::critical);
    f.open(QFile::ReadOnly);
    fCheck.check();
    QTextStream ifs(&f);
    Info b;
    ifs>>b;
    while (ifs.status()==QTextStream::Ok)
    {
        infos.push_back(b);
        ifs>>b;
    }
    f.close();
}

Updater::InfoIterator Updater::getBankInfo(const QString& s)
{
    return find_if(infos.begin(),infos.end(),[&](const Info& i)
    {
        return i.bankName()==s;
    });
}

Updater::InfoIterator Updater::getBankInfo(const QUrl& s)
{
    return find_if(infos.begin(),infos.end(),[&](const Info& i)
    {
        return i.siteUrl().host()==s.host();
    });
}

void Updater::zeroedCount() noexcept
{
    if (loader)
        delete loader;
    loader=nullptr;
    setCountZero();
}

bool Updater::processOne(const Rate& e)
{
    /*if (e->bankName()!="PrivatBank")
        return false;
    if (!loader)
    {
        loader=new QNetworkAccessManager(this);
        connect(loader,SIGNAL(finished(QNetworkReply*)),SLOT(endWork(QNetworkReply*)));
    }
    //loader->get(QNetworkRequest(QUrl("https://privatbank.ua")));
    loader->get(QNetworkRequest(QUrl("https://api.privatbank.ua/p24api/pubinfo?exchange&coursid=5")));
    return true;*/
    auto it=getBankInfo(e.second);
    if (infos.end()==it)
        return false;
    if (!loader)
    {
        loader=new QNetworkAccessManager(this);
        connect(loader,SIGNAL(finished(QNetworkReply*)),SLOT(endWork(QNetworkReply*)));
    }
    auto reply=loader->get(QNetworkRequest(it->siteUrl()));
    QObject::connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslError(QList<QSslError>)));
    //ErrorManagement::checkNetworkAccess(loader,"Network Connection",UpdateError::huge);
    return true;
}

void Updater::sslError(QList<QSslError> erl)//easy procesing-just ignoring(and in debug mode printing into console)
{
#ifndef NDEBUG
#ifdef HTML_CODE
    qDebug()<<"\n\n\n\n\nSsl Errors:\n";
    for (int i=0;i<erl.size();++i)
    {
        qDebug()<<erl[i].errorString();
    }
    qDebug()<<"\n\n"<<flush;
#endif
#endif
      QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
      reply->ignoreSslErrors();
}

void Updater::endWork(QNetworkReply* r)
{
    if (r->manager()!=loader || !r) //if loader was deleted before this function was called
        return;
    vector<ExchangeRate::ExchangeValute> valutes;
    //QXmlStreamReader reader(r);
    //QNetworkReply::NetworkError error=r->error();
    QString ss=r->readAll();
#ifndef NDEBUG
#ifdef HTML_CODE
    qDebug()<<"\n\n\n\n\n";
    qDebug()<<r->url().toString()<<'\n';
    qDebug()<<ss<<"\n\n"<<flush;
#endif
#endif
    //QString str=r->readAll();
    //qDebug()<<str<<flush;
    /*do
    {
        reader.readNext();
        bool in;
        if (!(QXmlStreamReader::StartElement==reader.tokenType() && reader.name()=="select") )
        {
            QStringRef r1=reader.name();
            QStringRef r2=reader.attributes().value("class");
             if (!( reader.attributes().value("class")=="selectBox course_select out_currency" ||
                  (in=reader.attributes().value("class")=="selectBox course_select in_currency") ) )
                continue;
        }
        while ( reader.readNext()!=QXmlStreamReader::EndElement && reader.name()=="select" )
        {
            if (!reader.attributes().hasAttribute("value"))
                continue;
            double value=reader.attributes().value("value").toDouble();
            reader.readNext();
            QStringRef s=reader.text();
            if ("Українська гривня"==s)
                type=Hryvnia;
            else if ("Долар США"==s)
                type=Dollar;
            else if ("Євро"==s)
                type=Dollar;
            else if ("Російський рубль"==s)
                type=Dollar;
            else
                continue;
            auto it=find_if(valutes.begin(), valutes.end(), [=](ExchangeRate::ExchangeValute ev)
            {
               return  ev.type==type;
            });
            if (it==valutes.end())
            {
                valutes.push_back(ExchangeRate::ExchangeValute());
                it=valutes.end();
                --it;
            }
            if (in)
                it->buy=value;
            else
                it->sell=value;
        }
    }
    while (!reader.atEnd());*/
    /*valutes.push_back(ExchangeRate::ExchangeValute({Hryvnia,1,1}));
    do
    {
        bool ok;
        reader.readNext();
        if (reader.name()!="exchangerate")
            continue;
        auto at=reader.attributes();
        auto type=getType(at.value("ccy"));
        if (static_cast<int>(type)==-1)
            continue;
        if (at.value("base_ccy")!="UAH")
                continue;
        valutes.push_back(ExchangeRate::ExchangeValute());
        auto p_valute=&(valutes.back());
        p_valute->type=type;
        p_valute->sell=at.value("sale").toDouble(&ok);
        if (!ok)
            continue;
        p_valute->buy=at.value("buy").toDouble(&ok);
        if (!ok)
            continue;
    }
    while (!reader.atEnd());*/
    //valutes=PrivatApiExtractor::work(r->readAll(),"privat.ext");
    /*PrivatApiExtractor ex("privatbank.ext",Hryvnia);
    valutes=ex.extract(ss);
    ExchangeRate* e=find("PrivatBank");*/
    ExchangeRate* e=nullptr;
    auto it=getBankInfo(r->url());
    try{
        e=find(it->bankName()).first;
        if (QNetworkReply::NoError!=r->error())
            ErrorManagement::checkNetworkAccess(loader,"Access to network",UpdateError::huge);
        ErrorManagement::checkNetworkReply(r,r->url().toString());
        unique_ptr<AbstractExtractor> ptr(it->useExtractor(base));
        valutes=ptr->extract(ss);
        if (valutes.size()==0)
            ErrorManagement::error(it->bankName(),"Empty rate");
    }
    catch (UpdateError& e)
    {
        e.title=r->url().toString();
        if (UpdateError::huge==e.errorType)
        {
            hugeErrorHandle(e);
            return;
        }
        smallErrorHandle(e);
        if (r->error()!=QNetworkReply::TemporaryNetworkFailureError)
        {
            decreaseCount();
            emit oneUnSuccessUpdate();
        }
        r->deleteLater();
        return;
    }
    catch (EndIteratorError)
    {
        emit oneUnSuccessUpdate();
        decreaseCount();
        r->deleteLater();
        return;
    }

    if (!e)
        return;
    e->lock();
    e->setRate(valutes);
    e->unlock();
    r->deleteLater();
    emit oneSuccessUpdate();
    decreaseCount();
}
