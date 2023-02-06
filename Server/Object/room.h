#ifndef ROOM_H
#define ROOM_H

#include "Object.h"
#include"Area.h"
class Room:public Object
{
    Q_OBJECT
public:
    Room();
    void Destruct();
    void setPos(Area* area,int x,int y){m_area=area;xPos=x;yPos=y;}//标记房间所处的区域位置，当房间被销毁时，需要设置此区域位置为空
    Area* getRoomPos(int& x,int& y){x=xPos,y=yPos;return m_area;}
    Area* getArea(){return m_area;}
    bool Load(QString path);
    bool Load();
    bool LoadRoom();
    QString roomDescription()const{return get("desc").toString();}
private:
    Area* m_area;
    int xPos;
    int yPos;
signals:

public slots:

};

#endif // ROOM_H
