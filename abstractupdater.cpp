#include "abstractupdater.h"
#include "exchangerate.h"
#include "algorithm"
#include "QEventLoop"
#include "paralelconverter.h"

using namespace std;

AbstractUpdater::Rate AbstractUpdater::find(QString bankName)
{
   auto it=find_if(rates.begin(), rates.end(), [bankName] (const Rate& r)
        { return r.second==bankName ? true : false; } );
   if (rates.end()==it)
       throw EndIteratorError();
   return *it;
}

void AbstractUpdater::setInterval(int timeInterval, bool updateNow)
{
    killTimer(timerId);
    interval=timeInterval;
    if (updateNow)
        startUpdate();
    else
        startTimer(timeInterval);
}

void AbstractUpdater::decreaseCount()
{
    --count;
    if (!count)
    {
        timerId=startTimer(interval);
        emit setLocker(false);
        emit endUpdating();
    }
}

void AbstractUpdater::forEach()
try{
    //killTimer(timerId);
    //zeroedCount();
    for (auto it=rates.begin(); it!=rates.end(); ++it)
    {
       if (processOne(*it))
           ++count;
    };
}
catch(const UpdateError& e)
{
    zeroedCount();
    smallErrorHandle(e);
    emit setLocker(false);
    emit endUpdating();
}

void AbstractUpdater::smallErrorHandle(const UpdateError &e)
{
    //QEventLoop a;
    //connect(this,SIGNAL(smallErrorHandled()),&a,SLOT(quit()));
    emit smallError(e);
    //a.exec();
}

void AbstractUpdater::hugeErrorHandle(const UpdateError& e)
{
    zeroedCount();
    count=1;//to use decreaseCount t not double code;
    decreaseCount();
    emit smallError(e);
}

void AbstractUpdater::deleteRate(QString bankName)
{
    auto pc=ParalelConverter::instance();
    pc->deleteMutex.lock();
    pc->deleteMutex.unlock();
    deleteOneRate(bankName);
    pc->deleteWakeAll();
}

void AbstractUpdater::deleteOneRate(const QString& bankName)
{
    rates.erase(find_if(rates.begin(),rates.end(),[&](const Rate& r)
    {
        return r.second==bankName;
    }));
}

void AbstractUpdater::startUpdate()
{
    zeroedCount();
    auto instance=ParalelConverter::instance();
    instance->updateStart.lock();
    emit beginingUpdating();
    emit setLocker(true);
    instance->waitUpdateStart();
    instance->updateStart.unlock();
    forEach();
}

void AbstractUpdater::clear()
{
    auto pc=ParalelConverter::instance();
    pc->deleteMutex.lock();
    pc->deleteMutex.unlock();
    clearRates();
    pc->deleteWakeAll();
}

void AbstractUpdater::clearRates()
{
    rates=Rates();
}
