#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include "repositorychooser.h"
#include "ui_repositorychooser.h"

RepositoryChooser::RepositoryChooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RepositoryChooser)
{
    ui->setupUi(this);

    QFile autostart(QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "git-annex/autostart"));
    if(autostart.exists()) {
        static QRegExp tilde("^~/(.*)$");
        autostart.open(QFile::ReadOnly);
        while(!autostart.atEnd()) {
            QString line = QString(autostart.readLine()).trimmed();
            if(tilde.exactMatch(line)) {
                line = QDir::home().absoluteFilePath(tilde.cap(1));
            }
            ui->listWidget->addItem(line);
        }
    }

}

RepositoryChooser::~RepositoryChooser()
{
    delete ui;
}

void RepositoryChooser::accept()
{
    QDialog::accept();
    emit accepted(ui->listWidget->selectedItems().first()->text());
}
