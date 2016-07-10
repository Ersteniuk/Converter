#ifndef BANKSETTINGS_H
#define BANKSETTINGS_H
#include <QSettings>
#include <QStringList>

class BankSettings:private QSettings
{
private:
    QString where;
    QStringList all;
public:
    BankSettings(const QString& companyName,const QString& programName,const QString& whereSave,const QStringList allBanks,
                 QObject* parent=0):QSettings(companyName,programName,parent),where(whereSave),all(allBanks)
            {}

    void saveCurBanks(const QStringList& sl)
        { setValue(where,sl); }
    QStringList getCurBanks() const
        { return value(where,all).toStringList(); }
};


#endif // BANKSETTINGS_H
