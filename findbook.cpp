#include "findbook.h"
#include "ui_findbook.h"
#include "bookmode.h"
#include "personal.h"
#include "paymode.h"
#include <QMessageBox>
#include"sqlite3.h"
#include<QDebug>
#include<vector>
#include<ctime>
using namespace std;
extern char* nowaccount;
extern sqlite3 *db;
int n=0;
struct showbook
{
    QString name;
    QString author;
    QString IBSN;
};
vector<showbook> savedata;
int getcurrenttime()
{
    time_t t=time(NULL);
    tm now;
    localtime_s(&now,&t);
    int year=now.tm_year+1900;
    int month=now.tm_mon+1;
    int day=now.tm_mday;
    int date=year*10000+month*100+day;
    return date;
}
const char * writerecord(char * name,char * id,char * IBSN,int indate)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"INSERT INTO record (bookname,IBSN,userid,outdate,indate) VALUES('%s','%s','%s',-1,%d)",name,IBSN,id,indate);
    return buf;
}
const char * borrowbooknumber(char * ibsn, int number)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"UPDATE book SET number=%d WHERE IBSN='%s'",number,ibsn);
    return buf;
}
const char * onlyname(char * bookname)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT name,author,IBSN FROM book WHERE name='%s'",bookname);
    return buf;
}
const char * getnumber(char * ibsn)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT number FROM book WHERE IBSN='%s'",ibsn);
    return buf;
}
const char * onlytype(char * type)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT name,author,IBSN FROM book WHERE type='%s'",type);
    return buf;
}
const char * both(char * bookname,char * type)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT name,author,IBSN FROM book WHERE name='%s' AND type='%s'",bookname,type);
    return buf;
}
findbook::findbook(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::findbook)
{
    n=0;
    ui->setupUi(this);
    qDebug()<<nowaccount;
    savedata.clear();
    const char * sql="SELECT name,author,IBSN FROM book";
    sqlite3_stmt *stmt;
    int rc;
    rc=sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        showbook temp;
        temp.name=QString((char*)sqlite3_column_text(stmt, 0));
        temp.author=QString((char*)sqlite3_column_text(stmt, 1));
        temp.IBSN=QString((char*)sqlite3_column_text(stmt, 2));
        savedata.push_back(temp);
    }
    int length=savedata.size();

    ui->pages->setAlignment(Qt::AlignCenter);

    showpages(length,1);
}

void findbook::showpages(int length,int pagenumber){
    int show_number=(pagenumber-1)*5;
    int shoow;
    for (int i=0;i<5;i++){
        shoow=show_number+i;
        if (shoow<length){
            switch(i){
            case(0):set1(savedata[shoow].name,savedata[shoow].author);break;
            case(1):set2(savedata[shoow].name,savedata[shoow].author);break;
            case(2):set3(savedata[shoow].name,savedata[shoow].author);break;
            case(3):set4(savedata[shoow].name,savedata[shoow].author);break;
            case(4):set5(savedata[shoow].name,savedata[shoow].author);break;
            default:QMessageBox::warning(nullptr,nullptr,"something wrong");
            }
        }
        else if (shoow>=length){
            switch(i){
            case(4):hide5();break;
            case(3):hide4();break;
            case(2):hide3();break;
            case(1):hide2();break;
            default:QMessageBox::warning(nullptr,nullptr,"something wrong");
            }
        }
    }
}

findbook::~findbook()
{
    delete ui;
}

//页面跳转按钮
void findbook::on_PersonalMode_Button_clicked()
{
    personal *p_personal_window=new personal;
    p_personal_window->show();  // 显示新窗口
    this->hide();  // 关闭当前窗口
}
void findbook::on_BookMode_Button_clicked()
{
    bookmode *p_book_window = new bookmode;
    p_book_window->show();
    this->hide();
}
void findbook::on_PayMode_Button_clicked()
{
    paymode *p_pay_window = new paymode;
    p_pay_window->show();
    this->hide();
}

//翻页
void findbook::on_raise_button_clicked()//向后翻页
{
    QString pages_number=ui->pages->text();
    bool ok;
    int intValue = pages_number.toInt(&ok);
    int length=savedata.size();
    int max_pages;
    if (length%5==0){
        max_pages=length/5;
    }
    else{
        max_pages=(length/5)+1;
    }
    if (intValue<max_pages){
        intValue += 1;
        n+=5;
        ui->pages->setText(QString::number(intValue));
        //qDebug()<<p_book;
        showpages(length,intValue);
        //qDebug()<<p_book;

    }
    ui->pages->setAlignment(Qt::AlignCenter);
}

void findbook::on_reduce_button_clicked()//向前翻页
{
    QString pages_number=ui->pages->text();
    bool ok;
    int intValue = pages_number.toInt(&ok);
    int length=savedata.size();
    int max_pages;
    if (length%5==0){
        max_pages=length/5;
    }
    else{
        max_pages=(length/5)+1;
    }
    if (intValue>1){
        intValue -= 1;
        n-=5;
        ui->pages->setText(QString::number(intValue));
        showpages(length,intValue);
    }
    else{
        ui->pages->setText(QString::number(intValue));
        ui->pages->setAlignment(Qt::AlignCenter);
    }
    ui->pages->setAlignment(Qt::AlignCenter);

    /*static QString MaxPages;
    MaxPages="10";//用数据库长度除以页面一次展示的数量，得到最大页面

    bool ok;
    int int_Max = MaxPages.toInt(&ok);
    QString pages_number=ui->pages->toPlainText();
    int intValue = pages_number.toInt(&ok);


    vector<showbook> savedata;
    const char * sql="SELECT name,author FROM book";
    sqlite3_stmt *stmt;
    int rc;
    rc=sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        showbook temp;
        temp.name=QString((char*)sqlite3_column_text(stmt, 0));
        temp.author=QString((char*)sqlite3_column_text(stmt, 1));
        savedata.push_back(temp);
    }
    //int length=savedata.size();
    if (intValue<=int_Max)
    {

        showpages();

        //更新里面的资讯11111
    }
    else{
        ui->pages->setPlainText(QString::number(intValue));
        ui->pages->setAlignment(Qt::AlignCenter);
    }*/
}

void findbook::on_pages_textChanged()
{
}

void findbook::showpages(){

}

void findbook::on_find_button_clicked()
{
    QString BookName=ui->label_input->text();
    qDebug()<<BookName;
    QString TypeName=ui->Type_Name_chose_2->currentText();
    qDebug()<<TypeName;
    n=0;
    std::string temp1=BookName.toStdString();
    std::string temp2=TypeName.toStdString();
    char * bookname=(char*)temp1.c_str();
    char * type=(char*)temp2.c_str();
    savedata.clear();
    sqlite3_stmt *stmt;;
    int rc;
    if (BookName=="" && TypeName=="")
    {
        qDebug()<<"both empty";
        //全空警告
        qDebug()<<nowaccount;
        const char * sql="SELECT name,author,IBSN FROM book";
        sqlite3_stmt *stmt;
        int rc;
        rc=sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            showbook temp;
            temp.name=QString((char*)sqlite3_column_text(stmt, 0));
            temp.author=QString((char*)sqlite3_column_text(stmt, 1));
            temp.IBSN=QString((char*)sqlite3_column_text(stmt, 2));
            savedata.push_back(temp);
        }
        int length=savedata.size();
        qDebug()<<length;
        int intValue=1;
        ui->pages->setText(QString::number(intValue));
        showpages(length,intValue);
        //ui->pages->setAlignment(Qt::AlignCenter);

        //showpages(length,1);
    }
    else
    {
        if (TypeName=="")
        {
            qDebug()<<"typename empty";
            rc = sqlite3_prepare_v2(db, onlyname(bookname), -1, &stmt, NULL);
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
            {
                showbook temp;
                temp.name = QString((char*)sqlite3_column_text(stmt, 0));
                temp.author = QString((char*)sqlite3_column_text(stmt, 1));
                temp.IBSN=QString((char*)sqlite3_column_text(stmt,2));
                savedata.push_back(temp);
            }
        }
        else
        {
            if (BookName=="")
            {
                qDebug()<<"bookname empty";

                rc = sqlite3_prepare_v2(db, onlytype(type), -1, &stmt, NULL);
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
                {
                    showbook temp;
                    temp.name = QString((char*)sqlite3_column_text(stmt, 0));
                    temp.author = QString((char*)sqlite3_column_text(stmt, 1));
                    temp.IBSN=QString((char*)sqlite3_column_text(stmt,2));
                    savedata.push_back(temp);
                }
            }
            else
            {
                qDebug()<<"no empty";
                rc = sqlite3_prepare_v2(db, both(bookname,type), -1, &stmt, NULL);
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
                {
                    showbook temp;
                    temp.name = QString((char*)sqlite3_column_text(stmt, 0));
                    temp.author = QString((char*)sqlite3_column_text(stmt, 1));
                    temp.IBSN=QString((char*)sqlite3_column_text(stmt,2));
                    savedata.push_back(temp);
                }
            }
        }
        int length=savedata.size();
        if (length>0){
            //ui->pages->setAlignment(Qt::AlignCenter);
            showpages(length,1);
        }
        else{
            QMessageBox::warning(nullptr,nullptr,"没有这本书");
        }
    }

}

const char * findsame(char * account,char * IBSN)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM record WHERE userid='%s' AND IBSN='%s' AND outdate=-1",account,IBSN);
    return buf;
}
const char * finborrownumber(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM record WHERE userid='%s' AND outdate=-1",id);
    return buf;
}
void findbook::Borrowing(QString bookname)
{
    qDebug()<<n;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认操作", "你确定要借出这本书吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes){
        if (false){     //拉取数据库判断是否有欠款11111
            QMessageBox::warning(nullptr, nullptr, "你仍有书逾期未还或欠款未缴纳，无法借书");
        }
        else
        {
            showbook temp=savedata[n];
            int indate=getcurrenttime();
            std::string tempbookname=temp.name.toStdString();
            std::string tempIBSN=temp.IBSN.toStdString();
            char * bookname=(char*)tempbookname.c_str();
            char * IBSN=(char*)tempIBSN.c_str();
            int rc;
            sqlite3_stmt* stmt;
            const char * sql=getnumber(IBSN);
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            rc = sqlite3_step(stmt);
            int number=sqlite3_column_int(stmt,0);
            if (number==0)
            {
                QMessageBox::warning(nullptr, nullptr, "这本书没有了");
            }
            else
            {
                const char * sql1=findsame(nowaccount,IBSN);
                rc = sqlite3_prepare_v2(db, sql1, -1, &stmt, nullptr);
                rc = sqlite3_step(stmt);
                if (rc==SQLITE_ROW)
                {
                    QMessageBox::warning(nullptr, nullptr, "你在借同一本书");
                }
                else
                {
                    const char * sql2=finborrownumber(nowaccount);
                    rc=sqlite3_prepare_v2(db,sql2,-1,&stmt,NULL);
                    int count=0;
                    while((rc=sqlite3_step(stmt))==SQLITE_ROW)
                    {
                        count++;
                    }
                    if (count>=3)
                    {
                        QMessageBox::warning(nullptr, nullptr, "你已到达最高同时借阅数");
                    }
                    else
                    {
                        number--;
                        rc=sqlite3_exec(db,borrowbooknumber(IBSN,number),NULL,NULL,NULL);
                        rc=sqlite3_exec(db,writerecord(bookname,nowaccount,IBSN,indate),NULL,NULL,NULL);
                        QMessageBox::warning(nullptr, nullptr, "借书成功！记得保护好书籍，按时归还哦！");
                    }
                }
            }
        }
    }
    else if (reply==QMessageBox::No){
        QMessageBox::warning(nullptr, nullptr, "欢迎借取其他书籍哦");
    }
}


void findbook::hide5(){
    ui->n5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n55->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->author5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk5->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk5->setEnabled(false);
}
void findbook::hide2(){
    ui->n2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n22->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->author2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk2->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk2->setEnabled(false);
}
void findbook::hide3(){
    ui->n3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n33->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->author3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk3->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk3->setEnabled(false);
}
void findbook::hide4(){
    ui->n4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n44->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->author4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk4->setEnabled(false);
}

void findbook::set1(QString Bookname,QString BookAuthor){
    ui->bookk->setText(Bookname);
    ui->author1->setText(BookAuthor);
}
void findbook::set2(QString Bookname,QString BookAuthor){
    ui->bookk2->setText(Bookname);
    ui->author2->setText(BookAuthor);
    ui->n2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n22->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->author2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk2->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk2->setEnabled(true);
}
void findbook::set3(QString Bookname,QString BookAuthor){
    ui->bookk3->setText(Bookname);
    ui->author3->setText(BookAuthor);
    ui->n3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n33->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->author3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk3->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk3->setEnabled(true);
}
void findbook::set4(QString Bookname,QString BookAuthor){
    ui->bookk4->setText(Bookname);
    ui->author4->setText(BookAuthor);
    ui->n4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n44->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->author4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk4->setEnabled(true);
}
void findbook::set5(QString Bookname,QString BookAuthor){
    ui->bookk5->setText(Bookname);
    ui->author5->setText(BookAuthor);
    ui->n5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n55->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->author5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk5->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk5->setEnabled(true);
}


void findbook::on_bookk_clicked()
{
    QString Bookkkk=ui->bookk->text();
    Borrowing(Bookkkk);
}


void findbook::on_pages_textEdited(const QString &arg1)
{
    QString pages_number=arg1;
    bool ok;
    int intValue = pages_number.toInt(&ok);
    n=(intValue-1)*5;
    int length=savedata.size();
    int max_pages;
    if (length%5==0){
        max_pages=length/5;
    }
    else{
        max_pages=(length/5)+1;
    }
    if (intValue!=NULL){
        if (intValue>max_pages){
            intValue=max_pages;
            ui->pages->setText(QString::number(intValue));
            showpages(length,intValue);
        }
        else{
            showpages(length,intValue);
        }
        ui->pages->setAlignment(Qt::AlignCenter);
    }
    else if (intValue==NULL){
        qDebug()<<"HI";
    }
}


void findbook::on_bookk5_clicked()
{
    QString Bookkkk=ui->bookk5->text();
    n+=4;
    Borrowing(Bookkkk);
    n-=4;
}


void findbook::on_bookk4_clicked()
{
    QString Bookkkk=ui->bookk4->text();
    n+=3;
    Borrowing(Bookkkk);
    n-=3;
}


void findbook::on_bookk3_clicked()
{
    QString Bookkkk=ui->bookk3->text();
    n+=2;
    Borrowing(Bookkkk);
    n-=2;
}


void findbook::on_bookk2_clicked()
{
    QString Bookkkk=ui->bookk2->text();
    n+=1;
    Borrowing(Bookkkk);
    n-=1;
}







