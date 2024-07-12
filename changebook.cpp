#include "changebook.h"
#include "ui_changebook.h"
#include "checkbook.h"
#include <QMessageBox>
#include"sqlite3.h"
extern char* nowaccount;
extern sqlite3*db;
struct book
{
    QString name;
    QString author;
    QString isbn;
    QString press;
    QString type;
    int number;
};

const char * getisbn3(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM book WHERE IBSN='%s'",id);
    return buf;
}
const char * putbook2(char * bookname,char * author,char * ibsn,char*press,char * type)
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ,buf,"INSERT INTO book(name,author,press,type,number) VALUES('%s','%s','%s','%s','%s',1)",bookname,author,ibsn,press,type);
    return buf;
}
const char* updateNickname5(char* id, char* newNickname)
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, buf, "UPDATE reader SET nickname='%s' WHERE id='%s'", newNickname, id);
    return buf;
}

changebook::changebook(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::changebook)
{
    ui->setupUi(this);
    sqlite3_stmt* stmt;
    int rc;
    std::string tmp=ISBN_E.toStdString();
    char * isbn=(char*)tmp.c_str();
    const char * sql = getisbn3(isbn);
    book test;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        test.name=QString((char*)sqlite3_column_text(stmt, 0));
        test.author = QString((char*)sqlite3_column_text(stmt, 1));
        test.isbn = QString((char*)sqlite3_column_text(stmt, 2));
        test.press=QString((char*)sqlite3_column_text(stmt, 3));
        test.type=QString((char*)sqlite3_column_text(stmt, 4));
        test.number=sqlite3_column_int(stmt,5);
    }
    std::string temp;
    while(test.number>0)
    {
        int tt=test.number%10;
        char cc=tt+48;
        temp=cc+temp;
        test.number=test.number/10;
    }
    QString number=QString::fromStdString(temp);
    ui->Author_Name_input->setText(test.author);
    ui->Book_Name_input_2->setText(test.name);
    ui->Press_Name_input->setText(test.press);
    ui->isbn_label222->setText(test.isbn);
    ui->number->setText(number);
    if (test.type=="文学作品，小说")
    {
        ui->Type_Name_chose->setCurrentIndex(1);
    }
    if (test.type=="童书")
    {
        ui->Type_Name_chose->setCurrentIndex(2);
    }
    if (test.type=="教科书")
    {
        ui->Type_Name_chose->setCurrentIndex(3);
    }
    if (test.type=="传记，回忆录")
    {
        ui->Type_Name_chose->setCurrentIndex(4);
    }
    if (test.type=="科幻")
    {
        ui->Type_Name_chose->setCurrentIndex(5);
    }
    if (test.type=="历史")
    {
        ui->Type_Name_chose->setCurrentIndex(6);
    }
    if (test.type=="哲学")
    {
        ui->Type_Name_chose->setCurrentIndex(7);
    }
    //ui->Type_Name_chose->setText(test.type);//设置为对应文字的选项，否则就是空白

}

changebook::~changebook()
{
    delete ui;
}

void changebook::on_back_button_clicked()
{
    checkbook *p_m_check_window=new checkbook;
    p_m_check_window->show();  // 显示新窗口
    this->hide();  // 关闭当前窗口
}

const char * getisbn(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM book WHERE IBSN='%s'",id);
    return buf;
}
const char * updatebook(char * isbn,char * name,char * author,char * press,char * type,int number)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"UPDATE book SET name='%s',author='%s',press='%s',type='%s',number=%d WHERE IBSN='%s'",name,author,press,type,number,isbn);
    return buf;
}
const char * deletebook(char * ibsn)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"DELETE FROM book WHERE IBSN='%s'",ibsn);
    return buf;
}
void changebook::on_change_button_clicked()
{
    QString AutorName=ui->Author_Name_input->text();
    QString BookName=ui->Book_Name_input_2->text();
    QString PressName=ui->Press_Name_input->text();
    QString TypeName=ui->Type_Name_chose->currentText();
    QString number=ui->number->text();
    std::string nn=number.toStdString();
    std::string str1=AutorName.toStdString();
    std::string str2=BookName.toStdString();
    std::string str4=PressName.toStdString();
    std::string str5=TypeName.toStdString();
    int tempnum=0;
    int len=nn.length();
    for (int i=0;i<len;i++)
    {
        tempnum=tempnum*10+nn[i]-48;
    }
    char* author,*bookname,*press,*type;
    std::string tmp=ISBN_E.toStdString();
    char * ibsn=(char*)tmp.c_str();
    author=(char*)str1.c_str();
    bookname=(char*)str2.c_str();
    press=(char*)str4.c_str();
    type=(char*)str5.c_str();
    if (AutorName.isEmpty() || BookName.isEmpty() || PressName.isEmpty() || TypeName.isEmpty()){
        QMessageBox::warning(nullptr, nullptr, "Something is empty");
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认操作", "你确定要添加这本书吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply==QMessageBox::Yes){
            int rc;

            //更新资料
            rc=sqlite3_exec(db,updatebook(ibsn,bookname,author,press,type,tempnum),NULL,NULL,NULL);
            qDebug()<<bookname<<author<<ibsn<<press<<type;
            QMessageBox::warning(nullptr, nullptr, "更改成功");
        }
    }
}


void changebook::on_None_button_clicked()
{
    std::string tmp=ISBN_E.toStdString();
    char * ibsn=(char*)tmp.c_str();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认操作", "你确定要删除这本书吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes){

        //删除操作
        int rc;
        rc=sqlite3_exec(db,deletebook(ibsn),NULL,NULL,NULL);
         QMessageBox::warning(nullptr, nullptr, "删除成功");
    }
}


void changebook::on_PersonalMode_Button_clicked()
{
    QMessageBox::warning(nullptr,nullptr,"请先完成图书更改，返回后切换页面");
}

void changebook::on_FindBookMode_Button_clicked()
{
    QMessageBox::warning(nullptr,nullptr,"请先完成图书更改，返回后切换页面");
}

