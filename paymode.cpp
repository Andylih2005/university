#include "paymode.h"
#include "ui_paymode.h"
#include "bookmode.h"
#include "findbook.h"
#include "personal.h"
#include"sqlite3.h"
extern char* nowaccount;
extern sqlite3* db;
paymode::paymode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::paymode)
{
    ui->setupUi(this);
    judge();
}
const char * getcreadit1(char * account)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT credit FROM reader WHERE id='%s'",account);
    return buf;
}
void paymode::judge(){
    //拉取reader数据库
    sqlite3_stmt* stmt;
    const char * sql;
    int rc;
    sql=getcreadit1(nowaccount);
    rc=sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
    rc=sqlite3_step(stmt);
    int credit=sqlite3_column_int(stmt,0);
    if(credit>0)
    {
        ui->Label_1->setText("书籍逾期未还，应缴纳");
        QString str=QString::number(credit);
        ui->Label_money->setText(str);
        ui->Label_2->setText("元");
    }
    else
    {
        ui->Label_1->setText("没有罚款");
        ui->Label_money->setText("");
        ui->Label_2->setText("");
    }
}

paymode::~paymode()
{
    delete ui;
}

//页面跳转按钮
void paymode::on_PersonalMode_Button_clicked()
{
    personal *p_personal_window=new personal;
    p_personal_window->show();  // 显示新窗口
    this->hide();  // 关闭当前窗口
}

void paymode::on_BookMode_Button_clicked()
{
    bookmode *p_book_window = new bookmode;
    p_book_window->show();
    this->hide();
}

void paymode::on_FindBookMode_Button_clicked()
{
    findbook *p_find_window = new findbook;
    p_find_window->show();
    this->hide();
}

