#include "cdatabasemanage.h"
#include"../../depends/CELLLog.h"
#include<QDebug>
#include<QSqlRecord>
#include<QSqlField>
#include<QJsonArray>
#include<QSqlError>
#include<QDate>
void CDatabaseManage::connectDb()
{
    _db=QSqlDatabase::addDatabase("QSQLITE","elims");
   // _db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
   // _db.setHostName("127.0.0.1");
   // _db.setUserName("root");
   // _db.setPassword("spring01");
    _db.setDatabaseName("elims");
    if(!_db.open()){
        CELLLog::Info("CDatabaseManage error on open database:%s",_db.lastError().text().toUtf8().data());
        qDebug()<<_db.lastError().text();
        return;
    }
    _db.exec("CREATE DATABASE IF NOT EXISTS elims");
  // _db.setDatabaseName("elims");
   _db.open();
    qDebug()<<"db opened";
    _dbOpen=true;
    QSqlQuery query(_db);
    if(!query.exec("create table if not exists sys_employee_login (id integer primary key AUTOINCREMENT, name  char(32) not null unique, password  char(32),last_login_time datetime ,last_login_ip char(16)  );")){
        qDebug()<<"query error:"<<query.lastError().text();
    }
    if(!query.exec("select id from sys_employee_login where name='admin';")){
        qDebug()<<"query error:"<<query.lastError().text();
    }
    if(!query.next()){
        if(!query.exec("insert into sys_employee_login(name, password) values( 'admin', 'c09e4c730a31cd5ed3548434f075ef8e');")){
            qDebug()<<"query error:"<<query.lastError().text();
        }
    }
}


bool CDatabaseManage::doSql(const QString &sql)
{
    QSqlQuery query(_db);
    if(!query.exec(sql)){
        _lastError=query.lastError().text();
        return false;
    }
    return  true;
}

CDatabaseManage::CDatabaseManage():
    _dbOpen(false)
{
    connectDb();
}

bool CDatabaseManage::doTranslate( QString &word, bool toCN)
{
    QSqlQuery query(_db);
    QString sql;
    if(toCN){
        sql=QString("select cn from dictionary where en = '%1'").arg(word);
    }
    else sql=QString("select en from dictionary where cn = '%1'").arg(word);
    if(!query.exec(sql)){
        qDebug()<<"翻译错误："<<query.lastError().text();
        return false;
    }
    if (query.next())
        word=query.value(0).toString();
    return true;
}
