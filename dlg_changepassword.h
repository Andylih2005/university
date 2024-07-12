#ifndef DLG_CHANGEPASSWORD_H
#define DLG_CHANGEPASSWORD_H

#include <QDialog>

namespace Ui {
class Dlg_ChangePassword;
}

class Dlg_ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_ChangePassword(QWidget *parent = nullptr);
    ~Dlg_ChangePassword();

private slots:

    void on_password_input_1_textChanged(const QString &arg1);

    void on_enter_button_clicked();

private:
    Ui::Dlg_ChangePassword *ui;
};

#endif // DLG_CHANGEPASSWORD_H
