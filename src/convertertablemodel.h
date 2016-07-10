#ifndef CONVERTERTABLEMODEL_H
#define CONVERTERTABLEMODEL_H

#include <QAbstractTableModel>

class ConverterTableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QString bankName;
    qint32 curN, pastN;
    const static int nColumns=4;
    enum Rows {valuteColumn, sellColumn, buyColumn, baseColumn};
public:
    explicit ConverterTableModel(QString bank,QObject *parent = 0):
        QAbstractTableModel(parent),curN(0)
            { changeBank(bank); }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int	rowCount(const QModelIndex &parent = QModelIndex()) const;
    int	columnCount(const QModelIndex &parent = QModelIndex()) const;
public slots:
    void changeBank(const QString& bank);
    void allDataChanged();
};

#endif // CONVERTERTABLEMODEL_H
