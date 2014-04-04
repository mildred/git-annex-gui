#ifndef SLIMLINEEDIT_H
#define SLIMLINEEDIT_H

#include <QWidget>

namespace Ui {
class SlimLineEdit;
}

class SlimLineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit SlimLineEdit(QWidget *parent = 0);
    ~SlimLineEdit();

    bool    isEditing() const;
    QString text() const;

signals:
    void textChanged(QString text);
    void editStarting();

public slots:
    void setText(QString text);
    void setEdit(bool edit = true);
    void setNormal();

private:
    Ui::SlimLineEdit *ui;
};

#endif // SLIMLINEEDIT_H
