#ifndef CLOGINMANAGER_H
#define CLOGINMANAGER_H

#include <QObject>
#include"../../depends/CELLClient.h"
#include"cuser.h"
#include"../Client/qjsoncmd.h"
#include<QMap>
class CLoginManager : public QObject
{
    Q_OBJECT
public:
    explicit CLoginManager(QObject *parent = nullptr);
    void doLogin(CELLClient *pClient, const QString& id, const QString &passwd);//处理登录
    void doReg(CELLClient *pClient, RegMsg regInfo);
    QMap< QString,CUser*>userMap()const{
        return _userMap;
    }
    QList<CUser*>userList()const{
        return _users;
    }
    void addUser(CUser *user);
    void removeUser(CUser *user);
    CUser *findUser(const QString& id);
private:
    static void onNewUserName(CUser* user,const QString& str,void*lm);
    static void onNewUserGender( CUser* user,const QString& str,void*lm);
    void initNewUser(CUser* user);
signals:
private:
    QMap< QString,CUser*>_userMap;

    QList<CUser*> _users;
};

#endif // CLOGINMANAGER_H
