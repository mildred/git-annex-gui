#include <QStandardPaths>
#include <QDebug>
#include "repowindow.h"
#include "repository.h"
#include "remoteconfig.h"
#include "ui_repowindow.h"

RepoWindow::RepoWindow(Repository *repo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RepoWindow),
    repo(repo)
{
    ui->setupUi(this);
    ui->remoteConfig->setRemote(repo);

    /*
    refreshTimer.setSingleShot(false);
    refreshTimer.setInterval(1000);
    refreshTimer.start();
    */

    connect(repo, SIGNAL(assistantStateChanged(bool)), ui->checkBox_assistant, SLOT(setChecked(bool)));
    connect(ui->checkBox_assistant, SIGNAL(clicked(bool)), repo, SLOT(runAssistant(bool)));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(ui->pushButton_webapp, SIGNAL(clicked()), repo, SLOT(runWebApp()));
    connect(ui->checkBox_direct, SIGNAL(clicked(bool)), repo, SLOT(setDirectMode(bool)));
    connect(ui->spinBox_numcopies, SIGNAL(editingFinished()), this, SLOT(setNumCopies()));
    connect(ui->checkBox_autostart, SIGNAL(clicked(bool)), this, SLOT(setGitAnnexAutostarted(bool)));
    //connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));

    refresh();
}

RepoWindow::~RepoWindow()
{
    delete ui;
}

void RepoWindow::refresh()
{
    qDebug() << "refresh";
    setWindowTitle(repo->path().absolutePath());
    ui->checkBox_assistant->setChecked(repo->isAssistantRunning());
    ui->checkBox_autostart->setChecked(isGitAnnexAutostarted());
    ui->checkBox_direct->setChecked(repo->isDirectMode());
    ui->spinBox_numcopies->setValue(oldNumCopies = repo->numCopies());
    ui->remoteConfig->refresh();
}

void RepoWindow::setNumCopies()
{
    if(oldNumCopies == ui->spinBox_numcopies->value()) return;
    repo->setNumCopies(oldNumCopies = ui->spinBox_numcopies->value());
}

void RepoWindow::setGitAnnexAutostarted(bool autostarted)
{
    if (isGitAnnexAutostarted() == autostarted) return;

    QString path = QDir(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)).filePath("autostart/git-annex.desktop");
    QFile f(path);
    if(autostarted) {
        f.open(QFile::WriteOnly);
        f.write("[Desktop Entry]\n"
                "Type=Application\n"
                "Version=1.0\n"
                "Name=Git Annex Assistant\n"
                "Comment=Autostart\n"
                "Terminal=false\n"
                "Exec=git-annex assistant --autostart\n"
                "Categories=\n");
    } else {
        f.remove();
        /*
        f.open(QFile::WriteOnly);
        f.write("[Desktop Entry]\n"
                "Type=Application\n"
                "Version=1.0\n"
                "Name=Git Annex Assistant\n"
                "Comment=Disable Autostart\n"
                "Terminal=false\n"
                "Exec=true\n"
                "Categories=\n");
                */
    }
}

bool RepoWindow::isGitAnnexAutostarted()
{
    foreach(QString path, QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation)) {
        if(QFile::exists(QDir(path).filePath("autostart/git-annex.desktop"))) {
            return true;
        }
    }
    return false;
}
