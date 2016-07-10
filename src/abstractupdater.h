#ifndef ABSTRACTUPDATER_H
#define ABSTRACTUPDATER_H

#include <QObject>
//#include <string>
#include <cstdint>
#include <utility>
#include "updateerror.h"
//class ExchangeRate;
#include "exchangerate.h"

class EndIteratorError
{};

typedef std::vector<std::pair<ExchangeRate*,QString>> forQtRate;

class AbstractUpdater : public QObject
{
    Q_OBJECT
public:
    typedef std::pair<ExchangeRate*,QString> Rate;
    typedef std::vector<Rate> Rates;
private:
    int32_t interval;
    int32_t timerId;
    int16_t count;
protected:
    Rates rates;
public:
    explicit AbstractUpdater(int32_t timeInterval, QObject *parent = 0):
        QObject(parent),interval(timeInterval),count(0)
            {}
    virtual ~AbstractUpdater()
        { emit setLocker(false); }

    int getInterval() const
        { return interval; }

    void deleteOneRate(const QString& bankName);
    void clearRates();
protected:
    virtual void zeroedCount() noexcept=0;
    virtual bool processOne(const Rate& r)=0;
    void decreaseCount();
    void setCountZero()
        { count=0; }
    Rate find(QString bankName);
    void hugeErrorHandle(const UpdateError& e);
    void smallErrorHandle(const UpdateError& e);
private:
    void timerEvent(QTimerEvent *)
    {
        killTimer(timerId);
        startUpdate();
    }
    void forEach();
signals:
    void smallError(UpdateError e);
    //void smallErrorHandled();
    void setLocker(bool);
    void msInterval(int32_t);
    void oneSuccessUpdate();
    void oneUnSuccessUpdate();
    void beginingUpdating();
    void endUpdating();
protected slots:
    void startUpdate();

public slots:
    void addRate(ExchangeRate* e,QString name)
        { rates.push_back(std::make_pair(e,name)); }
    void addRate(std::pair<ExchangeRate*,QString> r)
        { rates.push_back(r); }
    //void setRates(const std::vector<std::pair<ExchangeRate*,QString>>& r)
    void setRates(forQtRate r)
    {
        rates=r;
    }
    void deleteRate(QString bankName);
    void clear();
    void setInterval(int timeInterval,bool updateNow);
    void getIntervalSafe()
        { emit msInterval(interval); }
};



Q_DECLARE_METATYPE(forQtRate)

#endif // ABSTRACTUPDATER_H
