#ifndef USER_CENTER_H
#define USER_CENTER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class User_Center;
}
QT_END_NAMESPACE

class User_Center : public QWidget
{
    Q_OBJECT

public:
    User_Center(QWidget *parent = nullptr);
    ~User_Center();

private:
    Ui::User_Center *ui;
};
#endif // USER_CENTER_H
