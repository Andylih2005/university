#ifndef CHECKBOOK_H
#define CHECKBOOK_H

#include <QWidget>

class inputbook;
class UseMmanagement;

extern QString ISBN_E;
extern char* isbnE;

namespace Ui {
class checkbook;
}

class checkbook : public QWidget
{
    Q_OBJECT

public:
    explicit checkbook(QWidget *parent = nullptr);
    ~checkbook();

private slots:
    void on_PersonalMode_Button_clicked();

    void on_FindBookMode_Button_clicked();


    void on_reduce_button_clicked();

    void on_raise_button_clicked();

    void on_pages_textChanged();

    void on_pushButton_4_clicked();

    void on_search_button_clicked();

private:
    Ui::checkbook *ui;
};

#endif // CHECKBOOK_H
