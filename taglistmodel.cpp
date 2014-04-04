#include "taglistmodel.h"

TagListModel::TagListModel(QObject *parent) :
    QStringListModel(parent)
{
}

int TagListModel::rowCount(const QModelIndex &parent) const
{
    return QStringListModel::rowCount(parent) + 1;
}

QVariant TagListModel::data(const QModelIndex &index, int role) const
{
    int rows = QStringListModel::rowCount(index.parent());
    if(index.row() == rows) {
        return QVariant();
    } else {
        return QStringListModel::data(index, role);
    }
}

bool TagListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int rows = QStringListModel::rowCount(index.parent());
    if (role == Qt::EditRole && value.toString().isEmpty()) {
        return removeRow(index.row(), index.parent());
    } else if (index.row() == rows) {
        return insertRow(index.row()+1, index.parent())
                && QStringListModel::setData(index, value, role);
    } else {
        return QStringListModel::setData(index, value, role);
    }
}
