#include "dlg_welcome.h"
#include "ui_dlg_welcome.h"
#include"dlg_signup.h"
#include"dlg_changepassword.h"
#include"sqlite3.h"
#include<QDebug>
#include"inputbook.h"
#include"personal.h"
#include<QMessageBox>
extern char* nowaccount;
extern sqlite3* db;
const char * findcpp2sql(char test[50])
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, buf, "SELECT password FROM manger where id='%s'", test);
    return buf;
}
const char * findreader(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT password FROM reader where id='%s'",id);
    return buf;
}
Dlg_Welcome::Dlg_Welcome(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dlg_Welcome)
{
    ui->setupUi(this);
    setFixedSize(800,400);
    ui->LePassword->setEchoMode(QLineEdit::Password);
    ui->rdbtnUser->setChecked(1);
    ui->rdbtnAdmin->setChecked(0);


}

Dlg_Welcome::~Dlg_Welcome()
{
    delete ui;
}

void Dlg_Welcome::on_btnSign_clicked()
{
   Dlg_SignUp  *sgu = new Dlg_SignUp;
    sgu->show();
    hide();
}

//忘记密码
void Dlg_Welcome::on_pushButton_clicked()
{
    Dlg_ChangePassword *cpw = new Dlg_ChangePassword;
    cpw->show();
    hide();
}


void Dlg_Welcome::on_btnLog_clicked()
{
    QString AccString = ui->LeAccount->text();
    QString PasswString = ui->LePassword->text();
    char* idname,*password;
    sqlite3_stmt* stmt;
    int rc;
    std::string str1=AccString.toStdString();
    std::string str2=PasswString.toStdString();
    idname=(char*)str1.c_str();
    password=(char*)str2.c_str();
    if (ui->rdbtnAdmin->isChecked())
    {
        const char * sql = findcpp2sql(idname);
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            const unsigned char* name = sqlite3_column_text(stmt, 0);
            if (strcmp((const char*)name, (const char*)password) == 0)
            {
                qDebug()<<"correct";
                strcpy(nowaccount,(const char*)idname);
                qDebug()<<nowaccount;
                inputbook *ipb = new inputbook;
                ipb->show();
                this->hide();
            }
            else
            {
                qDebug() << "password wrong";
                QMessageBox::warning(nullptr, "wrong", "Wrong Password");
            }
        }
        else
        {
            QMessageBox::warning(nullptr, "wrong", "Wrong Account");
            qDebug()<< "id wrong";
        }
    }
        else if (ui->rdbtnUser->isChecked())
    {
        const char * sql = findreader(idname);
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            const unsigned char* name = sqlite3_column_text(stmt, 0);
            if (strcmp((const char*)name, (const char*)password) == 0)
            {
                qDebug()<< "correct";
                strcpy(nowaccount,(const char*)idname);
                qDebug()<<nowaccount;
                personal *psn = new personal;
                psn->show();
                this->hide();
            }
            else
            {
                qDebug() << "password wrong";
                QMessageBox::warning(nullptr, "wrong", "Wrong Password");
            }
        }
        else
        {
            qDebug()<< "id wrong";
            QMessageBox::warning(nullptr, "wrong", "Wrong Account");
        }
    }
}





void Dlg_Welcome::on_rdbtnAdmin_clicked(bool checked)
{
    if (ui->rdbtnAdmin && checked) {
        ui->rdbtnUser->setChecked(!checked);
    } else if (ui->rdbtnUser && checked) {
        ui->rdbtnAdmin->setChecked(!checked);
    }
}



