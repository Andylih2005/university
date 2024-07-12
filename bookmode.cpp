#include "bookmode.h"
#include "ui_bookmode.h"
#include "personal.h"
#include "findbook.h"
#include "paymode.h"
#include "sqlite3.h"
#include <QMessageBox>
#include<vector>
extern char* nowaccount;
struct UserRecord {
    QString username;
    QString bookname;
    QString isbn;
    int intime;
    int outtime;
    int out_of_day;
};
extern sqlite3 *db;
int askborrown=0;
int getcurrenttime3()
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
int nn=0;
time_t dateToTimeT(int year, int month, int day) {
    tm date = {0};
    date.tm_year = year - 1900; // 年份从1900年开始计算
    date.tm_mon = month - 1;    // 月份从0开始计算
    date.tm_mday = day;
    return mktime(&date);
}
int daysBetweenDates(int year1, int month1, int day1, int year2, int month2, int day2) {
    time_t time1 = dateToTimeT(year1, month1, day1);
    time_t time2 = dateToTimeT(year2, month2, day2);
    double seconds = difftime(time2, time1);
    return static_cast<int>(seconds / (60 * 60 * 24));
}
const char * addrecord(char * bookname,char * ibsn,char * id,int indate)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"INSERT INTO record (bookname,IBSN,userid,outdate,indate) VALUES('%s','%s','%s',-1,%d)",bookname,ibsn,id,indate);
    return buf;
}
const char * addmoney(char * account,int money)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"UPDATE reader SET credit=%d WHERE id='%s'",money,account);
    return buf;
}
const char * getcreadit(char * account)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT credit FROM reader WHERE id='%s'",account);
    return buf;
}
const char * getall(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM record WHERE userid='%s'",id);
    return buf;
}
const char * findborrowbook2(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT id FROM reader",id);
    return buf;
}
const char * rfindibsn(char * ibsn)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT number FROM book WHERE IBSN='%s'",ibsn);
    return buf;
}
const char * addnumber(char * ibsn,int number)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"UPDATE book SET number=%d WHERE IBSN='%s'",number,ibsn);
    return buf;
}
const char * addoutdate(char * ibsn,char * nowaccount,int outdate)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"UPDATE record SET outdate=%d WHERE IBSN='%s' AND userid='%s'",outdate,ibsn,nowaccount);
    return buf;
}
int borrowmoney=0;

std::vector<UserRecord> records;
std::vector<UserRecord> borrow_list;
std::vector<UserRecord> finish_list;


bookmode::bookmode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::bookmode)
{
    ui->setupUi(this);
    updatapages();

}

void bookmode::updatapages(){
    records.clear();
    borrow_list.clear();
    finish_list.clear();
    const char * sql=getall(nowaccount);
    sqlite3_stmt *stmt;
    int rc;
    rc=sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        UserRecord temp;
        temp.username=QString((char*)sqlite3_column_text(stmt, 2));
        temp.bookname=QString((char*)sqlite3_column_text(stmt, 0));
        temp.isbn=QString((char*)sqlite3_column_text(stmt, 1));
        temp.intime=sqlite3_column_int(stmt,4);;
        temp.outtime=sqlite3_column_int(stmt,3);
        records.push_back(temp);
    }
    sqlite3_finalize(stmt);
    int length = records.size();
    for (size_t i = 0; i < records.size(); ++i) {
        int outtime_judge= records[i].outtime;
        if (outtime_judge==-1){
            UserRecord aa;
            aa.username=records[i].username;
            aa.bookname=records[i].bookname;
            aa.isbn=records[i].isbn;
            aa.intime=records[i].intime;
            aa.outtime=records[i].outtime;
            aa.out_of_day=0;
            borrow_list.push_back(aa);
        }
        else{
            UserRecord bb;
            bb.username=records[i].username;
            bb.bookname=records[i].bookname;
            bb.isbn=records[i].isbn;
            bb.intime=records[i].intime;
            bb.outtime=records[i].outtime;
            finish_list.push_back(bb);
        }
    }

    if (borrow_list.size()==0)
    {
        hide_up1();
        hide_up2();
        hide_up3();
        hide_up4();
        hide_up5();
    }
    else{
        for (size_t i = 0;i<5;i++){
            if (i<borrow_list.size())
            {
                int show_day=b_timeint(borrow_list[i].intime,borrow_list[i].out_of_day);
                switch (i)
                {
                case(0):set_up1(borrow_list[i].bookname,QString::number(show_day));
                    if (borrow_list[i].out_of_day) ui->n11->setText("逾期");
                    else ui->n11->setText("借了");
                    break;
                case(1):set_up2(borrow_list[i].bookname,QString::number(show_day));
                    if (borrow_list[i].out_of_day) ui->n11_2->setText("逾期");
                    else ui->n11_2->setText("借了");
                    break;
                case(2):set_up3(borrow_list[i].bookname,QString::number(show_day));
                    if (borrow_list[i].out_of_day) ui->n11_3->setText("逾期");
                    else ui->n11_3->setText("借了");
                    break;
                /*case(3):set_up4(borrow_list[i].bookname,QString::number(show_day));
                    if (borrow_list[i].out_of_day) ui->n11_4->setText("逾期");
                    else ui->n11_4->setText("借了");
                    break;
                case(4):set_up5(borrow_list[i].bookname,QString::number(show_day));
                    if (borrow_list[i].out_of_day) ui->n11_6->setText("逾期");
                    else ui->n11_6->setText("借了");
                    break;*/
                default:
                    switch (i){
                    case(0):hide_up1();break;
                    case(1):hide_up2();break;
                    case(2):hide_up3();break;
                    case(3):hide_up4();break;
                    case(4):hide_up5();break;
                    }
                }
            }
            else{
                switch (i){
                case(0):hide_up1();break;
                case(1):hide_up2();break;
                case(2):hide_up3();break;
                case(3):hide_up4();break;
                case(4):hide_up5();break;
                }
            }
        }
    }

    if (finish_list.size()==0){
        hide_down1();
        hide_down2();
        hide_down3();
        hide_down4();
    }
    else{
        for (size_t i = 0;i<4;i++){
            if (i<finish_list.size()){
                switch (i){
                case(0):set_down1(finish_list[i].bookname);break;
                case(1):set_down2(finish_list[i].bookname);break;
                case(2):set_down3(finish_list[i].bookname);break;
                case(3):set_down4(finish_list[i].bookname);break;
                default:
                    switch (i){
                    case(0):hide_down1();break;
                    case(1):hide_down2();break;
                    case(2):hide_down3();break;
                    case(3):hide_down4();break;
                    }
                }
            }
            else{
                switch (i){
                case(0):hide_down1();break;
                case(1):hide_down2();break;
                case(2):hide_down3();break;
                case(3):hide_down4();break;
                }
            }
        }
    }
}

int bookmode::b_timeint(int  intime,int &out_of_day){
    int currenttime=getcurrenttime3();
    int inyear=intime/10000;
    int outyear=currenttime/10000;
    int inmonth=(intime/100)%100;
    int outmonth=(currenttime/100)%100;
    int inday=intime%100;
    int outday=currenttime%100;
    int borrow_duration=daysBetweenDates(inyear,inmonth,inday,outyear,outmonth,outday);
    if (borrow_duration > 14)
    {
        out_of_day=1;
        return (borrow_duration-14);
    }
    else
    {
        out_of_day=0;
        return (borrow_duration);
    }
}

bookmode::~bookmode()
{
    delete ui;
}

void bookmode::hide_up1(){
    ui->n111->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n11->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n1->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->day1->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk->setEnabled(false);
}
void bookmode::hide_up2(){
    ui->n111_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n11_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n1_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->day_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_2->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_2->setEnabled(false);
}
void bookmode::hide_up3(){
    ui->n111_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n11_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n1_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->day_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_3->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_3->setEnabled(false);
}void bookmode::hide_up4(){
    ui->n111_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n11_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n1_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->day_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_4->setEnabled(false);
}void bookmode::hide_up5(){
    ui->n111_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n11_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->n1_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->day_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_6->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->bookk_6->setEnabled(false);
}

void bookmode::set_up5(QString Bookname,QString bookday){
    ui->bookk_6->setText(Bookname);
    ui->day_6->setText(bookday);
    ui->n111_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n11_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n1_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_6->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_6->setEnabled(true);
}
void bookmode::set_up4(QString Bookname,QString bookday){
    ui->bookk_4->setText(Bookname);
    ui->day_4->setText(bookday);
    ui->n111_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n11_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n1_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_4->setEnabled(true);
}
void bookmode::set_up3(QString Bookname,QString bookday){
    ui->bookk_3->setText(Bookname);
    ui->day_3->setText(bookday);
    ui->day_3->setStyleSheet("QLabel { color : black; }");
    ui->n111_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n11_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n1_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_3->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_3->setEnabled(true);
}
void bookmode::set_up2(QString Bookname,QString bookday){
    ui->bookk_2->setText(Bookname);
    ui->day_2->setText(bookday);
    ui->day_2->setStyleSheet("QLabel { color : black; }");
    ui->n111_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n11_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->n1_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_2->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk_2->setEnabled(true);
}
void bookmode::set_up1(QString Bookname,QString bookday)
{
    ui->bookk->setText(Bookname);
    ui->day1->setText(bookday);
    ui->day1->setStyleSheet("QLabel { color : black; }");
    ui->n111->setStyleSheet("");
    ui->n11->setStyleSheet("");
    ui->n1->setStyleSheet("");
    ui->bookk->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->bookk->setEnabled(true);
}

void bookmode::hide_down1(){
    ui->d->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db->setEnabled(false);
}
void bookmode::hide_down2(){
    ui->d_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db_2->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db_2->setEnabled(false);
}
void bookmode::hide_down3(){
    ui->d_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db_3->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db_3->setEnabled(false);
}
void bookmode::hide_down4(){
    ui->d_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db_4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->db_4->setEnabled(false);
}

void bookmode::set_down1(QString Bookname){
    ui->db->setText(Bookname);
    ui->d->setStyleSheet("");
    ui->db->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); border: none;}");
    ui->db->setEnabled(true);
}
void bookmode::set_down2(QString Bookname){
    ui->db_2->setText(Bookname);
    ui->d_2->setStyleSheet("");
    ui->db_2->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->db_2->setEnabled(true);
}
void bookmode::set_down3(QString Bookname){
    ui->db_3->setText(Bookname);
    ui->d_3->setStyleSheet("");
    ui->db_3->setStyleSheet(" QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->db_3->setEnabled(true);
}
void bookmode::set_down4(QString Bookname){
    ui->db_4->setText(Bookname);
    ui->d_4->setStyleSheet("");
    ui->db_4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->db_4->setEnabled(true);
}
void bookmode::showpages(int length,int pagenumber)
{
    int show_number=(pagenumber-1)*4;
    int shoow;
    for (int i=0;i<4;i++)
    {
        shoow=show_number+i;
        if (shoow<length){
            switch(i){
            case(0):set_down1(finish_list[shoow].bookname);break;
            case(1):set_down2(finish_list[shoow].bookname);break;
            case(2):set_down3(finish_list[shoow].bookname);break;
            case(3):set_down4(finish_list[shoow].bookname);break;
            default:QMessageBox::warning(nullptr,nullptr,"something wrong");
            }
        }
        else if (shoow>=length)
        {
            switch(i)
            {
            case(3):hide_down4();break;
            case(2):hide_down3();break;
            case(1):hide_down2();break;
            default:QMessageBox::warning(nullptr,nullptr,"something wrong");
            }
        }
    }
}
//页面跳转按钮
void bookmode::on_PersonalMode_Button_clicked()
{
    personal *p_personal_window=new personal;
    p_personal_window->show();  // 显示新窗口
    this->hide();  // 关闭当前窗口
}

void bookmode::on_FindBookMode_Button_clicked()
{

    findbook *p_find_window = new findbook;
    p_find_window->show();
    this->hide();
}

void bookmode::on_PayMode_Button_clicked()
{
    paymode *p_pay_window = new paymode;
    p_pay_window->show();
    this->hide();
}

void bookmode::askreturn(QString bookname){
    int rc;
    sqlite3_stmt* stmt;
    int index=0;
    int number=0;
    index=nn;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认操作", "你确定还书吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes)
    {
        std::string tt=borrow_list[index].isbn.toStdString();
        char * temp=(char*)tt.c_str();
        const char * sql=rfindibsn(temp);
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        rc = sqlite3_step(stmt);
        //还书功能，判断是否还成功
        number=sqlite3_column_int(stmt,0);
        number++;
        rc=sqlite3_exec(db,addnumber(temp,number),NULL,NULL,NULL);
        int outdate=getcurrenttime3();
        rc=sqlite3_exec(db,addoutdate(temp,nowaccount,outdate),NULL,NULL,NULL);
        borrow_list[index].outtime=outdate;
        int inyear=borrow_list[index].intime/10000;
        int outyear=outdate/10000;
        int inmonth=(borrow_list[index].intime/100)%100;
        int outmonth=(outdate/100)%100;
        int inday=borrow_list[index].intime%100;
        int outday=outdate%100;
        int borrowday=daysBetweenDates(inyear,inmonth,inday,outyear,outmonth,outday);
        if (borrowday>14)
        {
            borrow_list[index].out_of_day=true;
        }
        if (borrowday)
        {     //拉取数据库判断是否有欠款11111
            QMessageBox::warning(nullptr, nullptr, "还书成功，但注意要缴纳逾期费用哦");
            //计算费用存入credit
            int money=(borrowday-14)*10;
            sql=getcreadit(nowaccount);
            rc=sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
            rc=sqlite3_step(stmt);
            int credit=sqlite3_column_int(stmt,0);
            credit+=money;
            rc=sqlite3_exec(db,addmoney(nowaccount,credit),NULL,NULL,NULL);
        }
        else
        {
            QMessageBox::warning(nullptr, nullptr, "归还成功，欢迎阅览其他书籍");
        }
        borrow_list.erase(borrow_list.begin()+index);
    }
    else if (reply==QMessageBox::No){
        QMessageBox::warning(nullptr, nullptr, "取消还书，不要忘记借阅期限哦");
    }
    updatapages();
}

void bookmode::on_bookk_clicked()
{
    QString bookname=ui->bookk->text();
    nn=0;
    askreturn(bookname);
}
void bookmode::on_bookk_2_clicked()
{
    QString bookname=ui->bookk_2->text();
    nn=1;
    askreturn(bookname);
}
void bookmode::on_bookk_3_clicked()
{
    QString bookname=ui->bookk_3->text();
    nn=2;
    askreturn(bookname);
}
void bookmode::on_bookk_4_clicked()
{
    QString bookname=ui->bookk_4->text();
    nn=3;
    askreturn(bookname);
}
void bookmode::on_bookk_6_clicked()
{
    QString bookname=ui->bookk_6->text();
    nn=4;
    askreturn(bookname);
}
const char * getnumber1(char * ibsn)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT number FROM book WHERE IBSN='%s'",ibsn);
    return buf;
}
const char * countborrownumber(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM record WHERE userid='%s' AND outdate=-1",id);
    return buf;
}
const char * borrowbooknumber1(char * ibsn, int number)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"UPDATE book SET number=%d WHERE IBSN='%s'",number,ibsn);
    return buf;
}
void bookmode::askborrow(QString bookname)
{
    int index=askborrown;
    qDebug()<<askborrown;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认操作", "你要重新借阅这本书吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes)
    {
        int index2=0;
        for (size_t i = 0; i < borrow_list.size(); i++) {
            if (borrow_list[i].isbn == finish_list[index].isbn)
            {
                index2 = 100;
            }
        }
        if (index2)
        {
            QMessageBox::warning(nullptr, nullptr, "这本书正在被你借着呢，请找下自己的背包哦");
        }
        else
        {
            sqlite3_stmt* stmt;
            const char * sql;
            int rc;
            sql=getcreadit(nowaccount);
            rc=sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
            rc=sqlite3_step(stmt);
            int credit=sqlite3_column_int(stmt,0);
            if (credit>0)
            {
                QMessageBox::warning(nullptr, nullptr, "请先缴纳罚款");
            }
            else    
            {
                std::string tbookname=finish_list[askborrown].bookname.toStdString();
                std::string tibsn=finish_list[askborrown].isbn.toStdString();
                char * bookname=(char *)tbookname.c_str();
                char * ibsn=(char*)tibsn.c_str();
                int indate=getcurrenttime3();
                const char * sql=getnumber1(ibsn);
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                rc = sqlite3_step(stmt);
                int number=sqlite3_column_int(stmt,0);
                if (number==0)
                {
                    QMessageBox::warning(nullptr, nullptr, "没有这本书了");
                }
                else
                {
                    const char * sql2=countborrownumber(nowaccount);
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
                        rc=sqlite3_exec(db,borrowbooknumber1(ibsn,number),NULL,NULL,NULL);
                        rc=sqlite3_exec(db,addrecord(bookname,ibsn,nowaccount,indate),NULL,NULL,NULL);
                        QMessageBox::warning(nullptr, nullptr, "重新借阅成功");
                    }
                }
            }
        }
    }
    else if (reply==QMessageBox::No){
        QMessageBox::warning(nullptr, nullptr, "取消借书，欢迎继续阅览");
    }
    updatapages();
    ui->pages->setText("1");
    askborrown=0;
}

void bookmode::on_db_clicked()
{
    QString bookname=ui->db->text();
    askborrow(bookname);
    qDebug()<<borrow_list.size();
}
void bookmode::on_db_2_clicked()
{
    QString bookname=ui->db_2->text();
    askborrown++;
    askborrow(bookname);
    qDebug()<<borrow_list.size();
}


void bookmode::on_db_3_clicked()
{
    QString bookname=ui->db_3->text();
    askborrown+=2;
    askborrow(bookname);
    qDebug()<<borrow_list.size();
}


void bookmode::on_db_4_clicked()
{
    QString bookname=ui->db_4->text();
    askborrown+=3;
    askborrow(bookname);
}

void bookmode::on_pages_textEdited(const QString &arg1)
{
    QString pages_number=arg1;
    bool ok;
    int intValue = pages_number.toInt(&ok);
    askborrown=(intValue-1)*4;
    int length=finish_list.size();
    int max_pages;
    if (length%4==0){
        max_pages=length/4;
    }
    else{
        max_pages=(length/4)+1;
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
}


void bookmode::on_reduce_button_clicked()
{
    QString pages_number=ui->pages->text();
    bool ok;
    int intValue = pages_number.toInt(&ok);
    int length=finish_list.size();
    int max_pages;
    if (length%4==0){
        max_pages=length/4;
    }
    else{
        max_pages=(length/4)+1;
    }
    if (intValue>1){
        intValue -= 1;
        askborrown-=4;
        ui->pages->setText(QString::number(intValue));
        showpages(length,intValue);
    }
    else{
        ui->pages->setText(QString::number(intValue));
        ui->pages->setAlignment(Qt::AlignCenter);
    }
    ui->pages->setAlignment(Qt::AlignCenter);
}


void bookmode::on_raise_button_clicked()
{
    QString pages_number=ui->pages->text();
    bool ok;
    int intValue = pages_number.toInt(&ok);
    int length=finish_list.size();
    int max_pages;
    if (length%4==0){
        max_pages=length/4;
    }
    else{
        max_pages=(length/4)+1;
    }
    if (intValue<max_pages){
        intValue += 1;
        askborrown+=4;
        ui->pages->setText(QString::number(intValue));
        showpages(length,intValue);

    }
    ui->pages->setAlignment(Qt::AlignCenter);
}

