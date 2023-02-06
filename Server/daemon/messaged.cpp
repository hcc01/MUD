#include "messaged.h"
#include"../Object/room.h"
#include"../Object/Area.h"
MessageD::MessageD()
{

}

void MessageD::sendEnvMsg(CUser *user)
{
    if(!user->isWake()) return;
    EnvirontmentMsg envMsg;
    QString str;
    int x,y;
    Room* room=(Room*)user->getEnvironment();
    Area* area=(Area*)room->getRoomPos(x,y);
    //区域信息，用于加载地图
    envMsg.setAreaMsg(area->getAreaName(),x,y);
    str=QString("%1").arg(room->getFileName());
    envMsg.setRoomFile(str);//设置房间文件名，用于客户端加载房间信息
    str="";
    QMap<QString,Object*>contents=room->getContents();
    for(int i=0;i<contents.size();i++){
        QList<QString> objs=contents.keys();
        if(objs.at(i)==user->id()) continue;
        envMsg.addObjectMsg(objs.at(i),contents.value(objs.at(i))->type());
    }
    user->sendMsg(envMsg.msgInfo());
}

void MessageD::tellUser(CUser *user, const QString &msg)
{
    if(!user->isWake()) return;
    NoticeMsg ntMsg(msg);
    user->sendMsg(ntMsg.msgInfo());
}
