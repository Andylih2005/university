#ifndef BOOKMODE_H
#define BOOKMODE_H

#include <QWidget>

class personal;
class findbook;
class paymode;

namespace Ui {
class bookmode;
}

class bookmode : public QWidget
{
    Q_OBJECT

public:
     bookmode(QWidget *parent = nullptr);
    ~bookmode();

private slots:
    void on_PersonalMode_Button_clicked();
    void on_FindBookMode_Button_clicked();

    void on_PayMode_Button_clicked();
    int b_timeint(int intime,int &out_of_day);
    void updatapages();

    void hide_up1();
    void hide_up2();
    void hide_up3();
    void hide_up4();
    void hide_up5();

    void set_up1(QString Bookname,QString bookday);
    void set_up2(QString Bookname,QString bookday);
    void set_up3(QString Bookname,QString bookday);
    void set_up4(QString Bookname,QString bookday);
    void set_up5(QString Bookname,QString bookday);

    void hide_down1();
    void hide_down2();
    void hide_down3();
    void hide_down4();

    void set_down1(QString Bookname);
    void set_down2(QString Bookname);
    void set_down3(QString Bookname);
    void set_down4(QString Bookname);

    void on_bookk_clicked();
    void askreturn(QString bookname);
    void askborrow(QString bookname);

    void on_bookk_2_clicked();

    void on_bookk_3_clicked();

    void on_bookk_4_clicked();

    void on_bookk_6_clicked();

    void on_db_clicked();

    void on_db_2_clicked();

    void on_db_3_clicked();

    void on_db_4_clicked();
    void showpages(int length,int pagenumber);

    void on_pages_textEdited(const QString &arg1);

    void on_reduce_button_clicked();

    void on_raise_button_clicked();

private:
    Ui::bookmode *ui;
};

#endif // BOOKMODE_H
