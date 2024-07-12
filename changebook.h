#ifndef CHANGEBOOK_H
#define CHANGEBOOK_H

#include <QWidget>

namespace Ui {
class changebook;
}

class changebook : public QWidget
{
    Q_OBJECT

public:
    explicit changebook(QWidget *parent = nullptr);
    ~changebook();

private slots:
    void on_back_button_clicked();


    void on_change_button_clicked();

    void on_None_button_clicked();

    void on_PersonalMode_Button_clicked();

    void on_FindBookMode_Button_clicked();

private:
    Ui::changebook *ui;
};

#endif // CHANGEBOOK_H
