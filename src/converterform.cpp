#include "converter.h"
#include "extractor.h"
#include <qmessagebox.h>
#include "errormanagement.h"
#include "converterform.h"
#include "ui_converterform.h"

ConverterForm::ConverterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConverterForm)
{
    ui->setupUi(this);
    setBankBox();
    setBoxes(ui->bankBox->currentText());
    connect(ui->resultButton,SIGNAL(clicked()),SLOT(convert()));
    connect(ui->bankBox,SIGNAL(currentIndexChanged(QString)),SLOT(setBoxes(QString)));
}

ConverterForm::~ConverterForm()
{
    delete ui;
}

void ConverterForm::convert()
try{
    auto instance=Converter::instance();
    Money from(ValuteType(ui->valuteBox->currentText()),ui->fromBox->value());
    Money to(ValuteType(ui->valuteBoxTo->currentText()));
    bool ok=instance->convert(from,to,ui->bankBox->currentText());
    if (!ok)
        ErrorManagement::error("Помилка конвертування", "Дане конвертування неможливе. Спробуйте оновити "
                              "курси або виберіть інше конвертування.", UpdateError::huge);
    else
        ui->resultLabel->setText(QString::number(to.value()));
}
catch (const UpdateError& e){
    emit convertError(e);
}

void ConverterForm::setBoxes(const QString &s)
{
    auto instance=Converter::instance();
    auto list=instance->stringValutes(s);

    QString cur=ui->valuteBox->currentText();
    ui->valuteBox->clear();
    ui->valuteBox->addItems(list);
    //if (list.contains(cur))
    ui->valuteBox->setCurrentText(cur);

    cur=ui->valuteBoxTo->currentText();
    ui->valuteBoxTo->clear();
    ui->valuteBoxTo->addItems(list);
    //if (list.contains(cur))
    ui->valuteBoxTo->setCurrentText(cur);
}

void ConverterForm::setBankBox()
{
    QStringList sl=Converter::instance()->banks();
    QString cur=ui->bankBox->currentText();
    ui->bankBox->clear();
    ui->bankBox->addItems(sl);
    //if (sl.contains(cur))
    ui->bankBox->setCurrentText(cur);
}
