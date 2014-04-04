#ifndef REPOSITORYCHOOSER_H
#define REPOSITORYCHOOSER_H

#include <QDialog>

namespace Ui {
class RepositoryChooser;
}

class RepositoryChooser : public QDialog
{
    Q_OBJECT

public:
    explicit RepositoryChooser(QWidget *parent = 0);
    ~RepositoryChooser();

signals:
    void accepted(QString path);

public slots:
    void accept();

private:
    Ui::RepositoryChooser *ui;
};

#endif // REPOSITORYCHOOSER_H
