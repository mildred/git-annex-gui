#include <QStringList>
#include "taglistmodel.h"
#include "tagdelegate.h"
#include "taglistwidget.h"

TagListWidget::TagListWidget(QWidget *parent) :
    QListView(parent),
    model(new TagListModel(this)),
    delegate(new TagDelegate(this))
{
    QStringList tags;
    tags << "first";
    tags << "middle";
    tags << "last";
    model->setStringList(tags);

    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    //setCursor(Qt::IBeamCursor);
    setModel(model);
    setItemDelegate(delegate);

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(emitTagsChangedSignal()));
    connect(model, SIGNAL(layoutChanged()), this, SLOT(emitTagsChangedSignal()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitTagsChangedSignal()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitTagsChangedSignal()));
}

void TagListWidget::setTags(QStringList tags)
{
    model->setStringList(tags);
}

void TagListWidget::emitTagsChangedSignal()
{
    QStringList newTags = model->stringList();
    foreach(QString oldTag, oldTags) {
        if(oldTag.isEmpty()) continue;
        if(!newTags.contains(oldTag)) emit tagRemoved(oldTag);
    }
    foreach(QString newTag, newTags) {
        if(newTag.isEmpty()) continue;
        if(!oldTags.contains(newTag)) emit tagAdded(newTag);
    }
    oldTags = newTags;
    emit tagsChanged(newTags);
}
