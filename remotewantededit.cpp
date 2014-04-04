#include "remotewantededit.h"
#include "remotewanteddialog.h"
#include "wantedexpr.h"
#include "ui_remotewantededit.h"

RemoteWantedEdit::RemoteWantedEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteWantedEdit),
    dialog(new RemoteWantedDialog(this))
{
    ui->setupUi(this);

    connect(ui->pushButton_wantedexpr, SIGNAL(clicked()), dialog, SLOT(show()));
    connect(dialog, SIGNAL(finished(int)), this, SLOT(emitWantedExprModifiedSignal()));
}

RemoteWantedEdit::~RemoteWantedEdit()
{
    delete ui;
}

void RemoteWantedEdit::setWantedExpr(WantedExpr *wantedExpr)
{
    this->wantedExpr = wantedExpr;
    dialog->setWantedExpr(wantedExpr);
    connect(wantedExpr, SIGNAL(expressionChanged(QString)), this, SLOT(refresh()));
    refresh();
}

void RemoteWantedEdit::refresh()
{
    ui->label_wantedexpr->setText(wantedExpr->description());
}

void RemoteWantedEdit::emitWantedExprModifiedSignal()
{
    emit wantedExprModified(wantedExpr->expression());
}

