#include "slimlineedit.h"
#include "ui_slimlineedit.h"

SlimLineEdit::SlimLineEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlimLineEdit)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_view);

    connect(ui->button_edit,     SIGNAL(clicked()),       this, SLOT(setEdit()));
    connect(ui->button_validate, SIGNAL(clicked()),       this, SLOT(setNormal()));
    connect(ui->lineEdit,        SIGNAL(returnPressed()), this, SLOT(setNormal()));
}

SlimLineEdit::~SlimLineEdit()
{
    delete ui;
}

bool SlimLineEdit::isEditing() const
{
    return ui->stackedWidget->currentIndex() == 1;
    //return ui->stackedWidget->currentWidget() == ui->page_edit;
}

QString SlimLineEdit::text() const
{
    if(isEditing()) {
        return ui->lineEdit->text();
    } else {
        return ui->label->text();
    }
}

void SlimLineEdit::setText(QString text)
{
    ui->lineEdit->setText(text);
    if (!isEditing()) {
        ui->label->setText(text);
    }
}

void SlimLineEdit::setEdit(bool edit)
{
    if (edit) {
        ui->lineEdit->setText(text());
        emit editStarting();
        //ui->stackedWidget->setCurrentWidget(ui->page_view);
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        QString oldText = ui->label->text();
        QString newText = text();
        ui->label->setText(newText);
        //ui->stackedWidget->setCurrentWidget(ui->page_edit);
        ui->stackedWidget->setCurrentIndex(0);
        if(oldText != newText) emit textChanged(newText);
    }
}

void SlimLineEdit::setNormal()
{
    setEdit(false);
}
