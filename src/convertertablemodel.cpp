#include "convertertablemodel.h"
#include "converter.h"
#include "converterrate.h"

QVariant ConverterTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||  Qt::DisplayRole!=role)
        return QVariant();
    auto instance=Converter::instance();
    auto rates=instance->valutes(bankName);
    if (index.row()>=rates.size())
        return QVariant();
    ExchangeValue rate=rates[index.row()];
    switch (index.column())
    {
    case valuteColumn:
        return QVariant(rate.type.toQString());
    case sellColumn:
        return QVariant(rate.sell);
    case buyColumn:
        return QVariant(rate.buy);
    case baseColumn:
        return QVariant(rate.base.toQString());
    default:
        return QVariant();
    }
}

QVariant ConverterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole!=role || (Qt::Vertical!=orientation && Qt::Horizontal!=orientation) )
        return QVariant();
    if (Qt::Vertical==orientation)
        return QVariant(section+1);
    switch (section) //Qt::Horizontal==orientation
    {
    case valuteColumn:
        return QVariant("Валюта");
    case sellColumn:
        return QVariant("Продаж");
    case buyColumn:
        return QVariant("Купівля");
    case baseColumn:
        return QVariant("Базис");
    default:
        return QVariant();
    }
}

int	ConverterTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    //auto instance=Converter::instance();
    //auto rates=instance->valutes(bankName);
    //return rates.size();
    return curN;
}

int	ConverterTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return nColumns;
}

void ConverterTableModel::changeBank(const QString& bank)
{
    auto instance=Converter::instance();
    auto rates=instance->valutes(bank);
    bool insertion=rates.size()>curN;
    bool removing=rates.size()<curN;
    if (insertion)
        beginInsertRows(QModelIndex(), curN, rates.size()-1);
    else if (removing)
        beginRemoveRows(QModelIndex(), rates.size(),curN-1);
    bankName=bank;
    curN=rates.size();
    if (insertion)
       endInsertRows();
    else if (removing)
       endRemoveRows();
    allDataChanged();
}

void ConverterTableModel::allDataChanged()
{
    if (!hasIndex(0,0))
        return;
    auto begin=index(0,0);
    auto end=index(curN-1,nColumns-1);
    emit dataChanged(begin,end);
}
