#include "bankdialog.h"
#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <algorithm>
#include <QPushButton>
using namespace std;

BankDialog::BankDialog(QString mainTitle,QStringList banks, QStringList defaultOk, QWidget *parent) :
    QDialog(parent), boxes(banks.size())
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(mainTitle);
    QVBoxLayout* mainLay=new QVBoxLayout;
    QHBoxLayout* buttonLay=new QHBoxLayout;
    QVBoxLayout* groupLay=new QVBoxLayout;

    auto gBotton=new QGroupBox(mainTitle);
    auto boxesIt=boxes.begin();
    for(auto banksIt=banks.begin(); banksIt!=banks.end(); ++banksIt, ++boxesIt)
    {
        *boxesIt=new QCheckBox(*banksIt);
        if (defaultOk.contains(*banksIt))
            (*boxesIt)->setChecked(true);
        else
            (*boxesIt)->setChecked(false);
        groupLay->addWidget(*boxesIt);
    }
    gBotton->setLayout(groupLay);

    QPushButton* cancelButton=new QPushButton("Cancel");
    connect(cancelButton,SIGNAL(clicked()),SLOT(end()));
    buttonLay->addWidget(cancelButton);
    QPushButton* okButton=new QPushButton("Ok");
    connect(okButton,SIGNAL(clicked()),SLOT(endWithoutUpdate()));
    buttonLay->addWidget(okButton);
    QPushButton* bestOkButton=new QPushButton("Ok and update rates");
    connect(bestOkButton,SIGNAL(clicked()),SLOT(endWithUpdate()));
    buttonLay->addWidget(bestOkButton);

    mainLay->addWidget(gBotton);
    mainLay->addLayout(buttonLay);
    setLayout(mainLay);
}

QStringList BankDialog::createList() const
{
    QStringList sl;
    for(auto boxesIt=boxes.begin(); boxesIt!=boxes.end(); ++boxesIt)
    {
        if ((*boxesIt)->isChecked())
            sl<<(*boxesIt)->text();
    }
    return sl;
}
