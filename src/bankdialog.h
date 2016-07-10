#ifndef BANKDIALOG_H
#define BANKDIALOG_H

#include <QDialog>
#include <vector>

class QCheckBox;

class BankDialog : public QDialog
{
    Q_OBJECT
private:
    std::vector<QCheckBox*> boxes;

    QStringList createList() const;
public:
    explicit BankDialog(QString mainTitle, QStringList banks, QStringList defaultOk, QWidget *parent = 0);

signals:
    void newBank(const QStringList&, bool isShouldUpdate);
public slots:
    void endWithoutUpdate()
    {
        hide();
        emit newBank(createList(),false);
        deleteLater();
    }
    void endWithUpdate()
    {
        hide();
        emit newBank(createList(),true);
        deleteLater();
    }
    void end()
    {
        hide();
        deleteLater();
    }
};

#endif // BANKDIALOG_H
