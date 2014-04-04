#ifndef REMOTEWANTEDDIALOG_H
#define REMOTEWANTEDDIALOG_H

#include <QDialog>
#include <QPointer>

class WantedExpr;
namespace Ui {
class RemoteWantedDialog;
}

class RemoteWantedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteWantedDialog(QWidget *parent = 0);
    ~RemoteWantedDialog();

    void setWantedExpr(WantedExpr *wantedExpr);

public slots:
    virtual void setVisible(bool visible);

private slots:
    void setMode(int mode);
    void setModeToManual();
    void setModeToAll();
    void setModeToNone();
    void setModeToCustom();
    void setCustomExpression(QString expr);
    void setIncludeOldVersion(bool include);
    void setIncludeFileCopies(bool include);
    void refresh();

private:
    Ui::RemoteWantedDialog *ui;
    QPointer<WantedExpr> wantedExpr;
};

#endif // REMOTEWANTEDDIALOG_H
