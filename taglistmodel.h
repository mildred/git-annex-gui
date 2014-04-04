#ifndef TAGLISTMODEL_H
#define TAGLISTMODEL_H

#include <QStringListModel>

class TagListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit TagListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // TAGLISTMODEL_H
