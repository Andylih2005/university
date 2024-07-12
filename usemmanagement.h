#ifndef USEMMANAGEMENT_H
#define USEMMANAGEMENT_H

#include <QWidget>

class inputbook;
class checkbook;

namespace Ui {
class usemmanagement;
}

class usemmanagement : public QWidget
{
    Q_OBJECT

public:
    explicit usemmanagement(QWidget *parent = nullptr);
    ~usemmanagement();

private slots:
    void on_PersonalMode_Button_clicked();

    void on_BookMode_Button_clicked();

    void on_reduce_button_clicked();

    void on_raise_button_clicked();
    void showpages(int length,int pagenumber);

    void on_pages_textChanged();

    void hide6();
    void hide5();
    void hide4();
    void hide3();
    void hide2();

    //void showpages();
    //void on_find_button_clicked();

    void set1(QString username,QString booknumber,QString own);
    void set2(QString username,QString booknumber,QString own);
    void set3(QString username,QString booknumber,QString own);
    void set4(QString username,QString booknumber,QString own);
    void set5(QString username,QString booknumber,QString own);
    void set6(QString username,QString booknumber,QString own);

    void on_delete_2_clicked();

    void on_pushButton_2_clicked();

    void on_pages_textEdited(const QString &arg1);

private:
    Ui::usemmanagement *ui;
};

#endif // USEMMANAGEMENT_H
