#ifndef DLG_SIGNUP_H
#define DLG_SIGNUP_H

#include <QDialog>

namespace Ui {
class Dlg_SignUp;
}

class Dlg_SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_SignUp(QWidget *parent = nullptr);
    ~Dlg_SignUp();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dlg_SignUp *ui;
};

#endif // DLG_SIGNUP_H
