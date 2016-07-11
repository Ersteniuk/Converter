#ifndef SAVER_H
#define SAVER_H
#include <QObject>
#include <vector>
#include <exchangerate.h>
#include "SPLib/splib.h"
#include "updateerror.h"

//clas wich real save rates into spreadsheet(aka splib::Spreadsheet
//and then this into a file using WriterIntoFormat
class Saver: public QObject
{
    Q_OBJECT
public:
    typedef std::vector<ExchangeRate> Rates;
private:
    Rates rates_;
    QString name;
    splib::Writer* writer;
    splib::SpreadsheetImpl sheet;

    void writeIntoSheet();
    void writeIntoFile();
public:
    Saver(Rates rates, QString filename):rates_(rates),name(filename)
        {}
    ~Saver()
    {
        delete writer;
    }

public slots:
    void work();

signals:
    void smallError(UpdateError e);
    void successSave();
};

#endif // SAVER_H
