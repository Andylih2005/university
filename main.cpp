#include "user_center.h"
#include"dlg_welcome.h"
#include"dlg_signup.h"
#include"dlg_changepassword.h"
#include <QApplication>
#include"sqlite3.h"
char * nowaccount=new char[50];
sqlite3 * db;
extern char* ISBN_E;

int main(int argc, char *argv[])
{
    int rc;
    QApplication a(argc, argv);
    sqlite3_open("data.db",&db);
    rc = sqlite3_exec(db, "CREATE TABLE manger(id VARCHAR(50) PRIMARY KEY,password VARCHAR(50))", NULL, NULL, NULL);
    rc = sqlite3_exec(db, "CREATE TABLE book(name VARCHAR(50) NOT NULL,author VARCHAR(50) NOT NULL,IBSN VARCHAR(50) PRIMARY KEY NOT NULL,press VARCHAR(50) NOT NULL,type VARCHAR(50) NOT NULL,number INT)", NULL, NULL, NULL);
    rc = sqlite3_exec(db, "CREATE TABLE reader(id VARCHAR(50) PRIMARY KEY ,password VARCHAR(50),nickname VARCHAR(50),credit INT)", NULL, NULL, NULL);
    rc = sqlite3_exec(db, "CREATE TABLE record(bookname VARCHAR(50),IBSN VARCHAR(50),userid VARCHAR(50),outdate INT,indate INT)",NULL,NULL,NULL);
    Dlg_Welcome dlg;
    dlg.show();
    return a.exec();

}
