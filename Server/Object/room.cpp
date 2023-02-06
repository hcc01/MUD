#include "room.h"
#include"Area.h"
REGISTER_CLASS(Room);
Room::Room()
{
   setType(ROOM);
}
void Room::Destruct(){
    Area* area;
    int x,y;
    area=(Area*)getRoomPos(x,y);
    if(area) area->setRoomOb(nullptr,x,y);
    Object::Destruct();
}
bool Room::Load( QString path){
    if(!Object::Load(path)) return false;
    return LoadRoom();
}
bool Room::Load(){
    if(!Object::Load()) return false;
    return LoadRoom();
}
bool Room::LoadRoom(){
    QString str,clsName,path,info;
    str=get("npc").toString();
    int n=str.indexOf(";");
    int m;
    while(n!=-1){
        info=str.left(n);
        str=str.mid(n+1);
        n=str.indexOf(";");
        m=info.indexOf("@");
        clsName=info.left(m);
        path=info.mid(m+1);
        Object* npc;
        npc=Object::CreateObject(clsName.toLatin1());
        if(!npc){
            qDebug()<<"can't create object;"<<clsName;
            continue;
        }
        npc->Load(path.toLatin1());
        npc->moveTo(this);
    }
    return true;
}

