#ifndef INPUTBOOK_H
#define INPUTBOOK_H

#include <QWidget>

class checkbook;
class UseMmanagement;

namespace Ui {
class inputbook;
}

class inputbook : public QWidget
{
    Q_OBJECT

public:
    explicit inputbook(QWidget *parent = nullptr);
    ~inputbook();

private slots:
    void on_BookMode_Button_clicked();

    void on_FindBookMode_Button_clicked();

    void on_pushButton_clicked();

private:
    Ui::inputbook *ui;
};

#endif // INPUTBOOK_H
