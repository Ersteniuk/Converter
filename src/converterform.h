#ifndef CONVERTERFORM_H
#define CONVERTERFORM_H

#include <QWidget>
#include "updateerror.h"

namespace Ui {
class ConverterForm;
}

class ConverterForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConverterForm(QWidget *parent = 0);
    ~ConverterForm();

private:
    Ui::ConverterForm *ui;
public slots:
    void convert();

    void setBoxes(const QString& s);
    void setBankBox();
signals:
    void convertError(const UpdateError& e);
};

#endif // CONVERTERFORM_H
