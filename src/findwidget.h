#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include "converter.h"
#include "updateerror.h"

class QComboBox;
class QLabel;

class FindWidget : public QWidget
{
    Q_OBJECT
public:
    typedef double (Converter::*FType) (ValuteType valute, bool& isFinded, ValuteType baseValute, QString*) const;
private:
    FType f;
    QComboBox* valutes;
    QComboBox* baseValutes;
    QLabel* label;
public:
    FindWidget(FType funct,QString str, QWidget *parent = 0);
public slots:
    void find();
signals:
    void findError(const UpdateError& e);
};

#endif // FINDWIDGET_H
