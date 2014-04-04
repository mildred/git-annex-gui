#ifndef TAGDELEGATE_H
#define TAGDELEGATE_H

#include <QStyledItemDelegate>

class TagDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TagDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void commit();
    void commitAndCloseEditor();
};

#endif // TAGDELEGATE_H
