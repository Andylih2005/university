#ifndef PAYMODE_H
#define PAYMODE_H

#include <QWidget>

class bookmode;
class personal;
class findbook;

namespace Ui {
class paymode;
}

class paymode : public QWidget
{
    Q_OBJECT

public:
    explicit paymode(QWidget *parent = nullptr);
    ~paymode();

private slots:
    void on_PersonalMode_Button_clicked();

    void on_BookMode_Button_clicked();

    void on_FindBookMode_Button_clicked();
    void judge();

private:
    Ui::paymode *ui;
};

#endif // PAYMODE_H
