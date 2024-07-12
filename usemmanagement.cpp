#include "usemmanagement.h"
#include "ui_usemmanagement.h"
#include"sqlite3.h"
#include "checkbook.h"
#include "inputbook.h"
#include<vector>
#include <QMessageBox>
extern sqlite3 *db;
extern char * nowaccount;
int managen=0;
struct read
{
    QString idname;
    QString book;
    QString own;
};
time_t dateToTimeT1(int year, int month, int day) {
    tm date = {0};
    date.tm_year = year - 1900; // 年份从1900年开始计算
    date.tm_mon = month - 1;    // 月份从0开始计算
    date.tm_mday = day;
    return mktime(&date);
}
int daysBetweenDates1(int year1, int month1, int day1, int year2, int month2, int day2) {
    time_t time1 = dateToTimeT1(year1, month1, day1);
    time_t time2 = dateToTimeT1(year2, month2, day2);
    double seconds = difftime(time2, time1);
    return static_cast<int>(seconds / (60 * 60 * 24));
}
std::vector<read> people;

const char * findborrowbook(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT id FROM reader",id);
    return buf;
}
const char * deletereader(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"DELETE FROM reader WHERE id='%s'",id);
    return buf;
}
int getcurrenttime2()
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
const char * findrecord(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"SELECT * FROM record WHERE userid='%s'",id);
    return buf;
}
usemmanagement::usemmanagement(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::usemmanagement)
{
    ui->setupUi(this);
    ui->pages->setAlignment(Qt::AlignCenter);
    QFont font = ui->pages->font();
    font.setPointSize(16);
    ui->pages->setFont(font);
        //使用当前idname，在record数据库中寻找是否有此idname的借书记录，如果有的话将同一条借书记录中的借书时间保存为intime、还书时间保存为outime，并计算当前这条借书记录的借书时间
        //如果outtime为空则假设为今天的日期
        //如果当前借书记录的  outtime-intime>14  则记为逾期，超过n天就令ownmoney添加1
        //如果当前借书记录的  outtime-intime<14  则记为借书期间，令borrowtime+=1

    people.clear();
    const char *sql = "SELECT id,credit FROM reader";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "Failed to prepare statement: " << sqlite3_errmsg(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        read temp;
        temp.idname = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        temp.own=QString::number(sqlite3_column_int(stmt,1));
        people.push_back(temp);
    }
    sqlite3_finalize(stmt);

    for (size_t i = 0; i < people.size(); ++i) {
        read &temp = people[i];
        std::string temp1=temp.idname.toStdString();
        char * id=(char*)temp1.c_str();
        // 查找 record 表中的借书记录
        const char *record_sql = findrecord(id);
        sqlite3_stmt *record_stmt;
        rc = sqlite3_prepare_v2(db, record_sql, -1, &record_stmt, nullptr);
        if (rc != SQLITE_OK) {
            qDebug() << "Failed to prepare record statement: " << sqlite3_errmsg(db);
            continue;
        }
        //sqlite3_bind_text(record_stmt, 1, temp.idname.toStdString().c_str(), -1, SQLITE_STATIC);

        int borrowtime = 0;
        bool hasRecords = false; // 用于检查是否有记录返回
        while (sqlite3_step(record_stmt) == SQLITE_ROW)
        {
            hasRecords = true;
            int intime = sqlite3_column_int(record_stmt, 4);
            int outtime = sqlite3_column_int(record_stmt, 3);
            if (outtime==-1)//未还书
            {
                borrowtime++;
                outtime=intime;
            }
            // 计算借书时间（天数）
        }

        if (!hasRecords) {
            qDebug() << "No records found for userid:" << temp.idname;
        }
        temp.book = QString::number(borrowtime);
        qDebug() << borrowtime;
        sqlite3_finalize(record_stmt);
    }
    int length = people.size();
    qDebug() << "Total people processed:" << length;
    showpages(length,1);

}

void usemmanagement::showpages(int length,int pagenumber){
    int show_number=(pagenumber-1)*5;
    int shoow;
    for (int i=0;i<6;i++){
        shoow=show_number+i;
        if (shoow<length){
            switch(i){
            case(0):set1(people[shoow].idname,people[shoow].book,people[shoow].own);break;
            case(1):set2(people[shoow].idname,people[shoow].book,people[shoow].own);break;
            case(2):set3(people[shoow].idname,people[shoow].book,people[shoow].own);break;
            case(3):set4(people[shoow].idname,people[shoow].book,people[shoow].own);break;
            case(4):set5(people[shoow].idname,people[shoow].book,people[shoow].own);break;
            case(5):set6(people[shoow].idname,people[shoow].book,people[shoow].own);break;
            default:QMessageBox::warning(nullptr,nullptr,"something wrong");
            }
        }
        else if (shoow>=length){
            switch(i){
            case(5):hide6();break;
            case(4):hide5();break;
            case(3):hide4();break;
            case(2):hide3();break;
            case(1):hide2();break;
            default:QMessageBox::warning(nullptr,nullptr,"something wrong");
            }
        }
    }
}
usemmanagement::~usemmanagement()
{
    delete ui;
}

void usemmanagement::on_PersonalMode_Button_clicked()
{
    inputbook *p_m_input_window=new inputbook;
    p_m_input_window->show();  // 显示新窗口
    this->hide();  // 关闭当前窗口
}


void usemmanagement::on_BookMode_Button_clicked()
{
    checkbook *p_m_check_window=new checkbook;
    p_m_check_window->show();  // 显示新窗口
    this->hide();  // 关闭当前窗口
}

void usemmanagement::on_reduce_button_clicked()
{
    QString pages_number=ui->pages->text();
    bool ok;
    int intValue = pages_number.toInt(&ok);
    int length=people.size();
    int max_pages;
    if (length%6==0){
        max_pages=length/6;
    }
    else{
        max_pages=(length/6)+1;
    }
    if (intValue>1){
        intValue -= 1;
        managen-=6;
        ui->pages->setText(QString::number(intValue));
        showpages(length,intValue);
    }
    else{
        ui->pages->setText(QString::number(intValue));
        ui->pages->setAlignment(Qt::AlignCenter);
    }
    ui->pages->setAlignment(Qt::AlignCenter);
}


void usemmanagement::on_raise_button_clicked()
{
    QString pages_number=ui->pages->text();
    bool ok;
    int intValue = pages_number.toInt(&ok);
    int length=people.size();
    int max_pages;
    if (length%6==0){
        max_pages=length/6;
    }
    else{
        max_pages=(length/6)+1;
    }
    if (intValue<max_pages){
        intValue += 1;
        managen+=6;
        ui->pages->setText(QString::number(intValue));
        //qDebug()<<p_book;
        showpages(length,intValue);
        //qDebug()<<p_book;

    }
    ui->pages->setAlignment(Qt::AlignCenter);
}

const char * deleterecord(char * id)
{
    static char buf[BUFSIZ]={0};
    sqlite3_snprintf(BUFSIZ,buf,"DELETE FROM record WHERE userid='%s'",id);
    return buf;
}
void usemmanagement::on_pages_textChanged()
{/*
    static QString MaxPages;
    MaxPages="10";//用数据库长度除以页面一次展示的数量，得到最大页面
    bool ok;
    int int_Max = MaxPages.toInt(&ok);
    QString pages_number=ui->pages->toPlainText();
    int intValue = pages_number.toInt(&ok);
    if (intValue<=int_Max){
        //更新里面的资讯
    }
    else{
        ui->pages->setPlainText(QString::number(int_Max));
        ui->pages->setAlignment(Qt::AlignCenter);
    }
*/
}


void usemmanagement::hide5(){
    ui->name_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->book_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->money_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->a_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->b_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->c_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->d_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->e_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_6->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_6->setEnabled(false);
}
void usemmanagement::hide6(){
    ui->name_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->book_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->money_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->a_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->b_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->c_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->d_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->e_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_7->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_7->setEnabled(false);
}
void usemmanagement::hide2(){
    ui->name_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->book_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->money_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->a_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->b_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->c_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->d_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->e_2->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_3->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_3->setEnabled(false);
}
void usemmanagement::hide3(){
    ui->name_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->book_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->money_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->a_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->b_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->c_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->d_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->e_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_4->setEnabled(false);}
void usemmanagement::hide4(){
    ui->name_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->book_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->money_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->a_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->b_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->c_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->d_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->e_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_5->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0); border: none; }");
    ui->delete_5->setEnabled(false);}

void usemmanagement::set1(QString username,QString booknumber,QString own){
    ui->name->setText(username);
    ui->book->setText(booknumber);
    ui->money->setText(own);
}
void usemmanagement::set2(QString username,QString booknumber,QString own){
    ui->name_2->setText(username);
    ui->book_2->setText(booknumber);
    ui->money_2->setText(own);
    ui->name_2->setStyleSheet("");
    ui->book_2->setStyleSheet("");
    ui->money_2->setStyleSheet("");
    ui->delete_3->setStyleSheet("");
    ui->delete_3->setEnabled(true);
}
void usemmanagement::set3(QString username,QString booknumber,QString own){
    ui->name_3->setText(username);
    ui->book_3->setText(booknumber);
    ui->money_3->setText(own);
    ui->name_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->book_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->money_3->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_4->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_4->setEnabled(true);
}
void usemmanagement::set4(QString username,QString booknumber,QString own){
    ui->name_4->setText(username);
    ui->book_4->setText(booknumber);
    ui->money_4->setText(own);
    ui->name_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->book_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->money_4->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_5->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_5->setEnabled(true);
}
void usemmanagement::set5(QString username,QString booknumber,QString own){
    ui->name_5->setText(username);
    ui->book_5->setText(booknumber);
    ui->money_5->setText(own);
    ui->name_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->book_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->money_5->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_6->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_6->setEnabled(true);
}
void usemmanagement::set6(QString username,QString booknumber,QString own){
    ui->name_6->setText(username);
    ui->book_6->setText(booknumber);
    ui->money_6->setText(own);
    ui->name_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->book_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->money_6->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_7->setStyleSheet("QPushButton { background-color: rgba(0, 0, 0, 0);}");
    ui->delete_7->setEnabled(true);
}

void usemmanagement::on_delete_2_clicked()
{
    int temp=ui->pages->text().toInt();
    std::string tt=people[(temp-1)*6].idname.toStdString();
    char * id=(char*)tt.c_str();
    int rc;
    rc=sqlite3_exec(db,deletereader(id),NULL,NULL,NULL);
    rc=sqlite3_exec(db,deleterecord(id),NULL,NULL,NULL);
}


void usemmanagement::on_pushButton_2_clicked()
{

}


void usemmanagement::on_pages_textEdited(const QString &arg1)
{
    QString pages_number=arg1;
    bool ok;
    int intValue = pages_number.toInt(&ok);
    managen=(intValue-1)*6;
    int length=people.size();
    int max_pages;
    if (length%6==0){
        max_pages=length/6;
    }
    else{
        max_pages=(length/6)+1;
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

