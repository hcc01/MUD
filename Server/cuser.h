#ifndef CUSER_H
#define CUSER_H

#include"../../depends/user.h"
#include<QString>
#include<QVector>
#include"./Object/Role.h"
class CUser:public Role,public CELLUser
{
public:
    CUser();
    CUser(const QString& ID, const QString& name,int gender);
    void OnDisconnect()override;
    QString id()const{ return get("ID").toString();}
    void setID(const QString& id){set("ID",id);}
    void setStartRoom(const QString& area,int x,int y);
    void movetoStartRoom();
    bool moveTo(int x, int y, const QString &areaname)override;

    void setReply(int f){m_replyTo=f;}
    void resetReply(){m_replyTo=-1;}
    int replyTo()const{return m_replyTo;}
private:
    QVector<CUser*> _friendsOnline;
    int m_replyTo;//处理回复信息的函数序号

};

#endif // CUSER_H
