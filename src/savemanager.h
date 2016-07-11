#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
#include  <QThread>
#include <QStringList>
#include "updateerror.h"

//Interface class for saving into different formats
//Saving do in a different thread, but this class is thread-safe
class SaveManager: public QObject
{
    Q_OBJECT
private:
    mutable QThread thread;
public:
    ~SaveManager();
    QStringList types() const;
public slots:
    //Save into format type with filename name. Filename should not include format
    void work(QString name,QString type) const
    {

    }
signals:
    void warning(UpdateError) const;
    void saveSuceed() const;
    void workSignal() const;//signal for internal use-start saving
};

#endif // SAVEMANAGER_H
