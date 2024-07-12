char * nowaccount=new char[50];
/*
const char * findcpp2sql(char test[50])
{
	static char buf[BUFSIZ] = { 0 };
	sqlite3_snprintf(BUFSIZ, buf, "SELECT password FROM manger where id='%s'", test);
	return buf;
}
const char * insertcpp2sql(char* id,char* password)
{
	static char buf[BUFSIZ] = { 0 };
	sqlite3_snprintf(BUFSIZ, buf, "INSERT INTO manger(id,password) VALUES('%s','%s')", id, password);
	return buf;
}
const char* changepasswordcpp2sql(char id[50],char password[50])
{
	static char buf[BUFSIZ] = { 0 };
	sqlite3_snprintf(BUFSIZ, buf, "UPDATE manger SET password='%s' WHERE id='%s'", password, id);
	return buf;
}
int print(void* data, int column, char ** values, char ** fileds)
{
    for (int i = 0; i < column; i++)
	{
            qDebug()<<values[i];
	}
	return SQLITE_OK;
}
/*
void test()
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	int rc;
	char idname[50];
	unsigned char * password = new unsigned char[50];
	rc = sqlite3_open("test.db", &db);
	rc = sqlite3_exec(db, "CREATE TABLE manger(id VARCHAR(50) PRIMARY KEY,password VARCHAR(50))", NULL, NULL, NULL);
	/*注册
	char nidname[50], npassword[50];
	cin >> nidname >> npassword;
	rc = sqlite3_exec(db, insertcpp2sql(nidname, npassword), NULL, NULL, NULL);
	if (!rc)
	{
		cout << "scusses";
	}
	else
	{
		cout << "fail";
	}
	//rc = sqlite3_exec(db, "SELECT * FROM manger", print, NULL, NULL);
	//登入
	cin >> idname >> password;
	const char * sql = findcpp2sql(idname);
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) 
	{
		const unsigned char* name = sqlite3_column_text(stmt, 0);
		if (strcmp((const char*)name, (const char*)password) == 0)
		{
			cout << "correct";
		}
		else
		{
			cout << "password wrong";
		}
	}
	else 
	{
		cout<< "id wrong" << endl;
	}
	char newpassword[50];
	cin >> newpassword;//輸入新密碼
	rc = sqlite3_exec(db, changepasswordcpp2sql(idname, newpassword), NULL, NULL, NULL);//執行
	rc = sqlite3_exec(db, "SELECT * FROM manger", print, NULL, NULL);
}*/
