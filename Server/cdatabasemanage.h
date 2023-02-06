#ifndef CDATABASEMANAGE_H
#define CDATABASEMANAGE_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QJsonObject>
#include"../Client/qjsoncmd.h"

#define ITEMS_PER_PAGE 10
class CDatabaseManage
{
public:
    static CDatabaseManage& Instance(){
        static CDatabaseManage dm;
        return dm;
    }
    QSqlDatabase& database(){
        return _db;
    }
    void connectDb();
    bool isOpen(){
        return _dbOpen;
    }
    QString lastError()const{
        return _lastError;
    }
    bool doSql(const QString &sql);


private:
    CDatabaseManage();
    bool doTranslate(QString &word, bool toCN=true);
private:
    QSqlDatabase _db;
    bool _dbOpen;
    QString _lastError;
};

#endif // CDATABASEMANAGE_H
