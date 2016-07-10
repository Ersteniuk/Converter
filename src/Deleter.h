#ifndef DELETER_H
#define DELETER_H
#include <QObject>

class Deleter:public QObject
{
    Q_OBJECT
public slots:
    void deletionSlot(QObject* dead)
    { delete dead; }
};

#endif // DELETER_H
