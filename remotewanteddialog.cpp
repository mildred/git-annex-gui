#include "wantedexpr.h"
#include "remotewanteddialog.h"
#include "ui_remotewanteddialog.h"

RemoteWantedDialog::RemoteWantedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoteWantedDialog)
{
    ui->setupUi(this);

    connect(ui->radioButton_all, SIGNAL(clicked()), this, SLOT(setModeToAll()));
    connect(ui->radioButton_custom, SIGNAL(clicked()), this, SLOT(setModeToCustom()));
    connect(ui->radioButton_manual, SIGNAL(clicked()), this, SLOT(setModeToManual()));
    connect(ui->radioButton_none, SIGNAL(clicked()), this, SLOT(setModeToNone()));
    connect(ui->lineEdit_customExpr, SIGNAL(textEdited(QString)), this, SLOT(setCustomExpression(QString)));
    connect(ui->checkBox_include_lackingcopies, SIGNAL(clicked(bool)), this, SLOT(setIncludeFileCopies(bool)));
    connect(ui->checkBox_include_old_versions, SIGNAL(clicked(bool)), this, SLOT(setIncludeOldVersion(bool)));
}

RemoteWantedDialog::~RemoteWantedDialog()
{
    delete ui;
}

void RemoteWantedDialog::setWantedExpr(WantedExpr *wantedExpr)
{
    this->wantedExpr = wantedExpr;
}

void RemoteWantedDialog::setVisible(bool visible)
{
    if(visible) refresh();
    QDialog::setVisible(visible);
}

void RemoteWantedDialog::setMode(int mode)
{
    wantedExpr->setMode(mode);
    refresh();
}

void RemoteWantedDialog::setModeToManual()
{
    setMode(WantedExpr::MANUAL);
}

void RemoteWantedDialog::setModeToAll()
{
    setMode(WantedExpr::ALL);
}

void RemoteWantedDialog::setModeToNone()
{
    setMode(WantedExpr::NONE);
}

void RemoteWantedDialog::setModeToCustom()
{
    setMode(WantedExpr::CUSTOM);
}

void RemoteWantedDialog::setCustomExpression(QString expr)
{
    wantedExpr->setMode(WantedExpr::CUSTOM, expr);
    refresh();
}

void RemoteWantedDialog::setIncludeOldVersion(bool include)
{
    wantedExpr->setIncludeOldVersions(include);
    refresh();
}

void RemoteWantedDialog::setIncludeFileCopies(bool include)
{
    wantedExpr->setIncludeFileCopies(include);
    refresh();
}

void RemoteWantedDialog::refresh()
{
    int mode = wantedExpr->mode();
    ui->radioButton_manual->setChecked(mode == WantedExpr::MANUAL);
    ui->radioButton_all->setChecked(mode == WantedExpr::ALL);
    ui->radioButton_none->setChecked(mode == WantedExpr::NONE);
    ui->radioButton_custom->setChecked(mode == WantedExpr::CUSTOM);

    ui->lineEdit_customExpr->setText(
                (mode == WantedExpr::CUSTOM) ?
                    wantedExpr->customExpression():
                    wantedExpr->expression());

    ui->checkBox_include_old_versions->setChecked(wantedExpr->includeOldVersions());
    ui->checkBox_include_lackingcopies->setChecked(wantedExpr->includeFileCopies());
}
