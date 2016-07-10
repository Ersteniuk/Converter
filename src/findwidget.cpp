#include "findwidget.h"
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <errormanagement.h>
#include "converter.h"

FindWidget::FindWidget(FType funct, QString str, QWidget *parent) :
    QWidget(parent), f(funct)
{
    setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout* mainVLay=new QVBoxLayout;
    QHBoxLayout* mainLay=new QHBoxLayout;
    mainVLay->addStretch();
    mainVLay->addWidget(new QLabel(str));
    QHBoxLayout* valuteLay=new QHBoxLayout;
    valuteLay->addWidget(new QLabel("Валюта"));
    valutes=new QComboBox;
    valutes->addItems(ValuteType::valutes());
    valuteLay->addWidget(valutes);
    mainVLay->addLayout(valuteLay);
    QHBoxLayout* baseLay=new QHBoxLayout;
    baseLay->addWidget(new QLabel("Базова валюта"));
    baseValutes=new QComboBox;
    baseValutes->addItems(ValuteType::valutes());
    baseLay->addWidget(baseValutes);
    mainVLay->addLayout(baseLay);
    QHBoxLayout* buttonLay=new QHBoxLayout;
    buttonLay->addWidget(label=new QLabel());
    QPushButton* button=new QPushButton("Пошук");
    connect(button,SIGNAL(clicked()),SLOT(find()));
    buttonLay->addWidget(button);
    mainVLay->addLayout(buttonLay);
    mainVLay->addStretch();
    mainLay->addStretch();
    mainLay->addLayout(mainVLay);
    mainLay->addStretch();
    setLayout(mainLay);
}

void FindWidget::find()
try{
    QString bank;
    bool isFinded;
    double val=(Converter::instance()->*f)(ValuteType(valutes->currentText()),isFinded,ValuteType(baseValutes->currentText()),&bank);
    if (!isFinded)
        ErrorManagement::error("Пошук","Пошук не успішний. Оберіть іншу валюту або перевірте підключення до мережі.");
    label->setText(bank+" Значення: "+QString::number(val,'f',3));
}
catch(const UpdateError& e) {
    emit findError(e);
}
