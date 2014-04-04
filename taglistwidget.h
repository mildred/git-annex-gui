#ifndef TAGLISTWIDGET_H
#define TAGLISTWIDGET_H

#include <QListWidget>

class TagListModel;
class TagDelegate;

class TagListWidget : public QListView
{
    Q_OBJECT
public:
    explicit TagListWidget(QWidget *parent = 0);

signals:
    void tagsChanged(QStringList);
    void tagAdded(QString tag);
    void tagRemoved(QString tag);

public slots:
    void setTags(QStringList tags);

private slots:
    void emitTagsChangedSignal();

private:
    TagListModel *model;
    TagDelegate  *delegate;
    QStringList   oldTags;
};

#endif // TAGLISTWIDGET_H
