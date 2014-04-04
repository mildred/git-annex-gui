#include <QPainter>
#include <QLineEdit>
#include <QStaticText>
#include "tagdelegate.h"

TagDelegate::TagDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void TagDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QSize s = QStyledItemDelegate::sizeHint(option, index);
    QRectF r = option.rect;
    r.setWidth(r.width()-4);
    r.setHeight(r.height()-4);
    r.setTop(r.top()+2);
    r.setLeft(r.left()+2);
    painter->setBrush(index.data(Qt::BackgroundRole).value<QBrush>());
    painter->drawRoundedRect(r, 6, 6, Qt::AbsoluteSize);
    painter->drawStaticText(r.left()+2, r.top(), QStaticText(index.data().toString()));

    /*
    QRectF close(r.right()-2-r.height()/2, r.top()+r.height()/4, r.height()/2, r.height()/2);
    painter->drawRect(close);
    painter->drawLine(close.left(), close.top(), close.right(), close.bottom());
    painter->drawLine(close.left(), close.bottom(), close.right(), close.top());
    */
    //QStyledItemDelegate::paint(painter, option, index);
    painter->restore();
}

QSize TagDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize s = QStyledItemDelegate::sizeHint(option, index);
    return QSize(s.width()+4/*+s.height()/2*/, s.height()+4);
}

QWidget *TagDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //return QStyledItemDelegate::createEditor(parent, option, index);
    QLineEdit *editor = new QLineEdit(parent);
    editor->setProperty("index", QVariant::fromValue(index));
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    connect(editor, SIGNAL(textChanged(QString)), this, SLOT(commit()));
    return editor;
}

void TagDelegate::commit()
{
    QLineEdit *editor = qobject_cast<QLineEdit *>(sender());
    QModelIndex index = editor->property("index").value<QModelIndex>();
    emit commitData(editor);
    emit sizeHintChanged(index);
}

void TagDelegate::commitAndCloseEditor()
{
    QLineEdit *editor = qobject_cast<QLineEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);//, QAbstractItemDelegate::EditNextItem);
}
