#ifndef RATEFORM_H
#define RATEFORM_H

#include <QWidget>

class QComboBox;
class ConverterTableModel;

class RateForm : public QWidget
{
    Q_OBJECT
private:
    QComboBox* bankBox;
    ConverterTableModel* model;
public:
    explicit RateForm(QWidget *parent = 0);
    void setBanks();
public slots:
    void setBankBox();
};

#endif // RATEFORM_H
