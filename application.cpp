#include <QApplication>
#include "application.h"
#include "repository.h"
#include "repowindow.h"
#include "repositorychooser.h"

Application::Application(QObject *parent) :
    QObject(parent)
{

}

void Application::createRepository(QString path)
{
    Repository *repo = new Repository(path);
    RepoWindow *win = new RepoWindow(repo);
    repo->setParent(win);
    win->show();
    this->win.reset(win);
}

int Application::exec()
{
    if(qApp->arguments().size() < 2) {
        chooser.reset(new RepositoryChooser());
        connect(chooser.data(), SIGNAL(accepted(QString)), this, SLOT(createRepository(QString)));
        chooser->show();

        return qApp->exec();
    } else {
        createRepository(qApp->arguments().value(1));

        return qApp->exec();
    }
}
