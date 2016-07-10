#include "rateform.h"
#include <QComboBox>
#include "convertertablemodel.h"
#include "converter.h"
#include <QTableView>
#include <QVBoxLayout>


RateForm::RateForm(QWidget *parent) :
    QWidget(parent)
{
    bankBox=new QComboBox;
    setBanks();
    model=new ConverterTableModel(bankBox->currentText(),this);
    connect(bankBox,SIGNAL(currentIndexChanged(QString)),model,SLOT(changeBank(QString)));
    QTableView* view=new QTableView;
    view->setModel(model);
    auto lay=new QVBoxLayout;
    lay->addWidget(bankBox);
    lay->addWidget(view);
    setLayout(lay);
}

void RateForm::setBankBox()
{
    setBanks();
    model->changeBank(bankBox->currentText());
}

void RateForm::setBanks()
{
    QStringList sl=Converter::instance()->banks();
    QString cur=bankBox->currentText();
    bankBox->clear();
    bankBox->addItems(sl);
    //if (sl.contains(cur))
    bankBox->setCurrentText(cur);
}
