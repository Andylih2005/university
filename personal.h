#ifndef PERSONAL_H
#define PERSONAL_H

#include <QWidget>

class bookmode;
class findbook;
class paymode;

namespace Ui {
class personal;
}

class personal : public QWidget
{
    Q_OBJECT

public:
    personal(QWidget *parent = nullptr);
    ~personal();

private slots:
    void on_BookMode_Button_clicked();

    void on_FindBookMode_Button_clicked();

    void on_PayMode_Button_clicked();

    void on_Update_Button_clicked();

    void on_password_input_textChanged();

private:
    Ui::personal *ui;
    QString enterpassword; // 用于存储实际密码


};

#endif // PERSONAL_H
