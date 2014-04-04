#ifndef REMOTEWANTEDEDIT_H
#define REMOTEWANTEDEDIT_H

#include <QWidget>
#include <QPointer>

class WantedExpr;
class RemoteWantedDialog;
namespace Ui {
class RemoteWantedEdit;
}

class RemoteWantedEdit : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteWantedEdit(QWidget *parent = 0);
    ~RemoteWantedEdit();

    void setWantedExpr(WantedExpr *wantedExpr);

signals:
    void wantedExprModified(QString expr);

private slots:
    void refresh();
    void emitWantedExprModifiedSignal();

private:
    Ui::RemoteWantedEdit *ui;
    RemoteWantedDialog   *dialog;
    QPointer<WantedExpr> wantedExpr;
};

#endif // REMOTEWANTEDEDIT_H
