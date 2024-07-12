#include "personal.h"
#include "ui_personal.h"
#include "bookmode.h"
#include "findbook.h"
#include "paymode.h"
#include <QInputDialog>
#include <QLineEdit>
#include<QDebug>
#include <QString>
#include <QMessageBox>
#include"sqlite3.h"
extern char* nowaccount;
QString Old_Password;
QString Old_Nickname;
extern sqlite3 *db;
const char * getnickname(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT nickname FROM reader WHERE id='%s'",id);
    return buf;
}
const char * getpassword(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT password FROM reader WHERE id='%s'",id);
    return buf;
}
const char* updateNickname(char* id, char* newNickname)
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, buf, "UPDATE reader SET nickname='%s' WHERE id='%s'", newNickname, id);
    return buf;
}

const char* updatePassword(char* id, char* newPassword)
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, buf, "UPDATE reader SET password='%s' WHERE id='%s'", newPassword, id);
    return buf;
}

personal::personal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::personal)
    ,enterpassword("")

{
    sqlite3_stmt* stmt,*stmt1;
    char* account=new char[50];
    int rc;
    const char * findnick = getnickname(nowaccount);
    const char * findpassword=getpassword(nowaccount);
    rc = sqlite3_prepare_v2(db, findnick, -1, &stmt, nullptr);
    const unsigned char* nickname;
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        nickname = sqlite3_column_text(stmt, 0);
    }
    rc = sqlite3_prepare_v2(db, findpassword, -1, &stmt1, nullptr);
    rc = sqlite3_step(stmt1);
    const unsigned char* password;
    if (rc == SQLITE_ROW)
    {
        password = sqlite3_column_text(stmt1, 0);
    }
    Old_Password=(char *)password;
    Old_Nickname=(char *)nickname;
    ui->setupUi(this);
    ui->nickname_label->setText(Old_Nickname);
    ui->acc_label->setText(nowaccount);
    qDebug()<<Old_Nickname<<nowaccount;
}

personal::~personal()
{
    delete ui;
}

//页面跳转按钮
void personal::on_BookMode_Button_clicked()
{
    bookmode *p_book_window = new bookmode;
    p_book_window->show();
    this->hide();
}

void personal::on_FindBookMode_Button_clicked()
{
    findbook *p_find_window = new findbook;
    p_find_window->show();
    this->hide();
}

void personal::on_PayMode_Button_clicked()
{
    paymode *p_pay_window = new paymode;
    p_pay_window->show();
    this->hide();
}


void personal::on_Update_Button_clicked()
{
    bool ok;
    QString old_password_input = QInputDialog::getText(this, "check",
                                                "Enter old password:", QLineEdit::Normal,
                                                "", &ok);
    if (old_password_input==Old_Password){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Modify Options");
        msgBox.setText("Choose an option to modify:");
        QAbstractButton* pButtonNickname = msgBox.addButton("Modify Nickname", QMessageBox::YesRole);
        QAbstractButton* pButtonPassword = msgBox.addButton("Modify Password", QMessageBox::NoRole);
        QAbstractButton* pButtonCancel = msgBox.addButton("Cancel", QMessageBox::RejectRole);
        msgBox.exec();
        int rc;
        if (msgBox.clickedButton() == pButtonNickname) {
            // 用户选择了修改昵称
            bool ok;
            QString newNickname = QInputDialog::getText(this, "Modify Nickname",
                                                        "Enter new nickname:", QLineEdit::Normal,
                                                        "", &ok);
            if (ok && !newNickname.isEmpty()) {
                ui->nickname_label->setText(newNickname);
                // 上传去数据库1
                char* nidname = nowaccount;
                std::string strNewNickname = newNickname.toStdString();
                char* newNicknameCStr = (char*)strNewNickname.c_str();
                const char* sql = updateNickname(nidname, newNicknameCStr);
                qDebug() << "Executing SQL:" << sql;  // 新增部分：调试信息
                rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    qDebug() << "SQL error (nickname update):" << sqlite3_errmsg(db);  // 新增部分：调试信息
                    QMessageBox::warning(this, "Error", "Failed to update nickname.");
                } else {
                    QMessageBox::information(this, "Success", "Nickname updated successfully.");
                }
            }
            else{
                QMessageBox::warning(nullptr, "Empty", "Empty");
            }
        } else if (msgBox.clickedButton() == pButtonPassword) {// 用户选择了修改密码
            bool ok;
            QString newPassword = QInputDialog::getText(this, "Modify Password",
                                                        "Enter new password:", QLineEdit::Password,
                                                        "", &ok);
            if (ok && !newPassword.isEmpty()) {
                // 上传去数据库2
                char* nidname = nowaccount;
                std::string strNewPassword = newPassword.toStdString();
                char* newPasswordCStr = (char*)strNewPassword.c_str();
                const char* sql = updatePassword(nidname, newPasswordCStr);
                qDebug() << "Executing SQL:" << sql;  // 新增部分：调试信息
                rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    qDebug() << "SQL error (password update):" << sqlite3_errmsg(db);  // 新增部分：调试信息
                    QMessageBox::warning(this, "Error", "Failed to update password.");
                } else
                {
                    Old_Password=newPassword;
                    QMessageBox::information(this, "Success", "Password updated successfully.");
                }

            }
            else{
                QMessageBox::warning(nullptr, "Empty", "Empty");
            }
        } else if (msgBox.clickedButton() == pButtonCancel) {// 用户选择了取消修改，不执行任何操作
        }
        sqlite3_close(db); // 关闭数据库连接

    }
    else{
        QMessageBox::warning(nullptr, "wrong", "Wrong Password");
    }
}


void personal::on_password_input_textChanged()
{/*
    QString currentText = ui->password_input->toPlainText();
    if (currentText.length() > enterpassword.length()) {
        QChar newChar = currentText.right(1).at(0);
        enterpassword.append(newChar);
        ui->password_input->blockSignals(true);
        ui->password_input->setPlainText(QString('*').repeated(enterpassword.length()));
        ui->password_input->blockSignals(false);
        QTextCursor cursor = ui->password_input->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->password_input->setTextCursor(cursor);
    } else if (currentText.length() < enterpassword.length()) {
        enterpassword.chop(1);
    }*/
}

