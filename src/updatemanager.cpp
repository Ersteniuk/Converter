#include "updatemanager.h"
#include <QThread>
#include "abstractupdater.h"
#include "paralelconverter.h"

UpdateManager::~UpdateManager()
{
    if (updater)
        updater->deleteLater();
    if (thread)
        thread->deleteLater();
}

UpdateManager::UpdateManager(AbstractUpdater* startUpdater,QObject *parent):QObject(parent),
    updater(startUpdater),thread(nullptr)
{

    thread=new QThread;
    updater->moveToThread(thread);
    connect(updater,SIGNAL(setLocker(bool)),ParalelConverter::instance(),SLOT(setLocker(bool)));
    connect(ParalelConverter::instance(),SIGNAL(wantDelete(QString)),updater,SLOT(deleteRate(QString)));
    connect(ParalelConverter::instance(),SIGNAL(wantDeleteAll()),updater,SLOT(clear()));

    connect(this,SIGNAL(updateSignal()),updater,SLOT(startUpdate()));
    connect(this,SIGNAL(deleteRateSignal(QString)),updater,SLOT(deleteRate(QString)));
    connect(this,SIGNAL(addRateSignal(ExchangeRate*,QString)),updater,SLOT(addRate(ExchangeRate*,QString)));
    connect(this,SIGNAL(addRateSignal(std::pair<ExchangeRate*,QString>)),
            updater,SLOT(addRate(std::pair<ExchangeRate*,QString>)));
    connect(this,SIGNAL(setRatesSignal(forQtRate)),
            updater,SLOT(setRates(forQtRate)));
    connect(this,SIGNAL(setIntervalSignal(int,bool)),updater,SLOT(setInterval(int,bool)));
    connect(updater,SIGNAL(oneSuccessUpdate()),SIGNAL(oneSuccessUpdate()));
    connect(updater,SIGNAL(oneUnSuccessUpdate()),SIGNAL(oneUnSuccessUpdate()));
    connect(updater,SIGNAL(beginingUpdating()),SIGNAL(beginingUpdating()));
    connect(updater,SIGNAL(endUpdating()),SIGNAL(endUpdating()));
    connect(updater,SIGNAL(smallError(UpdateError)),SIGNAL(warning(UpdateError)));
    //connect(this,SIGNAL(warningFinished()),updater,SIGNAL(smallErrorHandled()));
    thread->start();
 //   emit updateSignal();
}

void UpdateManager::addRate(ExchangeRate* e)
    { emit addRateSignal(e,e->bankName()); }

void UpdateManager::setRates(const std::vector<ExchangeRate*>& ps)
{
    std::vector<std::pair<ExchangeRate*,QString>> rs(ps.size());
    auto it=ps.begin();
    std::for_each(rs.begin(),rs.end(), [&](AbstractUpdater::Rate& r)
    {
        r=std::move(std::make_pair(*it,std::move((*it)->bankName())));
        ++it;
    });
    emit setRatesSignal(rs);
}
