#include "converter.h"
#include "extractor.h"
#include <qmessagebox.h>
#include "mainwindow.h"
#include <QTabWidget>
#include "errormanagement.h"
#include "ui_mainwindow.h"
#include "converterform.h"
#include "rateform.h"
#include <QSignalMapper>

MainWindow::MainWindow(const QString &progName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    setWindowTitle(progName);
    QMenu* file=new QMenu("&Файл");
    file->addAction("&Оновити курси валют",this,SIGNAL(updateSignal()),QKeySequence("Ctrl+U"));
    file->addAction("&Налаштування",this,SIGNAL(settings()));
    file->addSeparator();
    file->addAction("&Вихід",qApp,SLOT(quit()),QKeySequence("Ctrl+X"));
    ui->menuBar->addMenu(file);
    QMenu* find=new QMenu("&Пошук");
    QSignalMapper* map=new QSignalMapper(this);
    connect(map,SIGNAL(mapped(int)),SIGNAL(find(int)));
    QAction* a;
    a=find->addAction("Максимальна вартість продажу",map,SLOT(map()));
    map->setMapping(a,maxSellType);
    a=find->addAction("Максимальна вартість купівлі",map,SLOT(map()));
    map->setMapping(a,maxBuyType);
    a=find->addAction("Мінімальна вартість продажу",map,SLOT(map()));
    map->setMapping(a,minSellType);
    a=find->addAction("Мінімальна вартість купівлі",map,SLOT(map()));
    map->setMapping(a,minBuyType);
    ui->menuBar->addMenu(find);
    auto tabWidget=new QTabWidget;
    auto cForm=new ConverterForm(tabWidget);
    auto rForm=new RateForm(tabWidget);
    tabWidget->addTab(cForm,"&Конвертер");
    tabWidget->addTab(rForm,"Курси &валют");
    setCentralWidget(tabWidget);
    connect(cForm,SIGNAL(convertError(UpdateError)),SIGNAL(convertError(UpdateError)));
    connect(this,SIGNAL(banksMayChange()),cForm,SLOT(setBankBox()));
    connect(this,SIGNAL(banksMayChange()),rForm,SLOT(setBankBox()));
    //cForm->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
