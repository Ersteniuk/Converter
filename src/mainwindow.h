#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "updateerror.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum FindKind {maxSellType, maxBuyType, minSellType, minBuyType};

    explicit MainWindow(const QString& progName,QWidget *parent = 0);
    ~MainWindow();
public slots:
    //void setConvertForm();
signals:
    void updateSignal();
    void convertError(const UpdateError& e);
    void settings();
    void banksMayChange();
    void find(int);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
