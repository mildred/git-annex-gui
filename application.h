#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QScopedPointer>
#include "repositorychooser.h"
#include "repowindow.h"

class RepoWindow;
class RepositoryChooser;

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = 0);

public slots:
    int exec();
    void createRepository(QString path);

private:
    QScopedPointer<RepoWindow>  win;
    QScopedPointer<RepositoryChooser> chooser;

};

#endif // APPLICATION_H
