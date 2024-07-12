#ifndef FINDBOOK_H
#define FINDBOOK_H

#include <QWidget>

class bookmode;
class personal;
class paymode;

namespace Ui {
class findbook;
}

class findbook : public QWidget
{
    Q_OBJECT

public:
    explicit findbook(QWidget *parent = nullptr);
    ~findbook();

private slots:
    void on_PersonalMode_Button_clicked();

    void on_BookMode_Button_clicked();

    void on_PayMode_Button_clicked();

    void on_raise_button_clicked();

    void on_reduce_button_clicked();

    void on_pages_textChanged();

    void hide5();
    void hide4();
    void hide3();
    void hide2();

    void showpages();
    void on_find_button_clicked();

    void set1(QString Bookname,QString BookAuthor);
    void set2(QString Bookname,QString BookAuthor);
    void set3(QString Bookname,QString BookAuthor);
    void set4(QString Bookname,QString BookAuthor);
    void set5(QString Bookname,QString BookAuthor);
    void showpages(int length,int pagenumber);

    void Borrowing(QString bookname);

    void on_bookk_clicked();

    void on_pages_textEdited(const QString &arg1);

    void on_bookk5_clicked();

    void on_bookk4_clicked();

    void on_bookk3_clicked();

    void on_bookk2_clicked();


private:
    Ui::findbook *ui;
};

#endif // FINDBOOK_H
