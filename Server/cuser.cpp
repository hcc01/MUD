#include "cuser.h"
#include "./daemon/messaged.h"
#include <processthreadsapi.h>
CUser::CUser():m_replyTo(-1)
{
    qDebug()<<"User() thead id:"<<GetCurrentProcessId();
    setType(USER);
}

CUser::CUser(const QString &ID, const QString &name, int gender):m_replyTo(-1)
{
     qDebug()<<"User(3) thead id:"<<GetCurrentProcessId();
    setType(USER);
    set("ID",ID);
    set("name",name);
    set("gender",gender);
    setCloneName(ID);//用户不是克隆物品，用ID替代（克隆名用于查找物品实体，克隆物品在克隆名在克隆时产生（CreateObject()）
}

void CUser::OnDisconnect()
{
    // delete this;
}

void CUser::setStartRoom(const QString &area, int x, int y)
{
    set("startRoom",QString("%1@%2,%3").arg(area).arg(x).arg(y));
}

void CUser::movetoStartRoom()
{
    QString str=get("startRoom").toString();
    int n=str.indexOf("@");
    if(n<1){
        qDebug()<<"startRoom error:"<<this->id()<<str;
        return;
    }
    int x,y;
    QString area=(str.left(n));
    str=str.mid(n+1);
    n=str.indexOf(",");
    x=str.left(n).toInt();
    y=str.mid(n+1).toInt();
    moveTo(x,y,area);

}

bool CUser::moveTo(int x, int y, const QString &areaname)
{
    qDebug()<<"moving user"<<this->getCloneName();
    if(Object::moveTo(x,y,areaname)){
        MessageD::sendEnvMsg(this);
        return true;
    }
    return false;
}
