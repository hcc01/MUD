#include "Area.h"
#include<QDebug>
#include<QFile>
#include<QDir>
#include"Room.h"
#include<QRandomGenerator>
Area::Area():
    m_length(1)
  ,m_width(1)
{
    for(int i=0;i<MAX_LEN;i++){
        for(int j=0;j<MAX_LEN;j++){
            RoomInfo* rm=new RoomInfo;
            m_roomInfo[i][j]=rm;
            m_RoomOb[i][j]=nullptr;
        }
    }
}

Area::~Area()
{
    for(int i=0;i<MAX_LEN;i++){
        for(int j=0;j<MAX_LEN;j++){
            delete m_roomInfo[i][j];
            if(m_RoomOb[i][j]) m_RoomOb[i][j]->Destruct();
        }
    }
}
QMap<QString,Area*> Area::areaList;
Area* Area::getArea(QString name){

    Area* area=areaList.value(name);
    if(!area){
        area=new Area;
        if(!area->Load(QString("./area/%1.a").arg(name))){
            if(area) delete area;
            area=NULL;
        }
    }
    qDebug()<<areaList;
    return area;
}

void Area::Serialize(QDataStream &ar, SERIA_TYPE st){
    switch(st){

    RoomInfo* rm;
    case SAVE:
        ar<<m_areaName<<m_length<<m_width;
        for(int i=0;i<m_length;i++){
            for(int j=0;j<m_width;j++){
                rm=m_roomInfo[i][j];
                ar<<rm->m_room<<rm->m_objs<<rm->m_direction<<rm->m_centerFlag<<rm->m_areaLinkDir<<rm->m_areaLinkAddr;
            }
        }
        break;
    case LOAD:
        ar>>m_areaName>>m_length>>m_width;
        for(int i=0;i<m_length;i++){
            for(int j=0;j<m_width;j++){
                rm=m_roomInfo[i][j];
                ar>>rm->m_room>>rm->m_objs>>rm->m_direction>>rm->m_centerFlag>>rm->m_areaLinkDir>>rm->m_areaLinkAddr;
            }
        }
        break;
    }

}

void Area::setSize(int x, int y){
    if(!checkSize(x,y)) return;
    m_length=x;
    m_width=y;
}

void Area::setRoom(int x, int y, const QString &s){
    if(x-1>=m_length||y-1>=m_width) return;
    m_roomInfo[x-1][y-1]->m_room=s;
}
void Area::setRoomOb(Object *room, int x, int y){
    if(x-1>=m_length||y-1>=m_width) return;
    m_RoomOb[x-1][y-1]=room;
    if(room) ((Room*)room)->setPos(this,x,y);
}
Object *Area::getRoomOb(int x, int y){

    if(x-1>=m_length||y-1>=m_width) return NULL;
    Object* room= m_RoomOb[x-1][y-1];

    if(!room){
        QString name=getRoom(x,y);

        room=Object::CreateObject(name);
        if(!room){
            qDebug()<<m_areaName<<x<<y<<name<<"wrong room class name";
            return NULL;
        }
        if(!room->Load(name)){
            qDebug()<<m_areaName<<x<<y<<"wrong room path";
            return NULL;
        }
        QStringList objs=getObjects(x,y);

        if(objs.size()) {
            qDebug()<<QString("%1(%2,%3)载入物品：").arg(m_areaName).arg(x).arg(y)<<objs;
            for(int i=0;i<objs.size();i++){//载入物品
                QStringList obj=objs.at(i).split("@");
                if(obj.size()!=3){
                    qDebug()<<"error in loading obj :"<<obj;
                    continue;
                }
                else{
                    QString file;int amount;int chance;
                    file=obj.at(0);
                    amount=obj.at(1).toInt();
                    chance=obj.at(2).toInt();
                    if(QRandomGenerator::global()->bounded(0,1000000)>chance) continue;
                    for(int i=0;i<amount;i++){
                        Object* ob=Object::CreateObject(file);
                        if(ob) ob->moveTo(room);
                    }
                }
            }
        }
        setRoomOb(room,x,y);
    }

    return room;
}

QString Area::getRoom(int x, int y){
    if(x-1>=m_length||y-1>=m_width) return "";
    return m_roomInfo[x-1][y-1]->m_room;
}
QString Area::getRoom(QPoint p){
    qDebug()<<"pos"<<p;
    return getRoom(p.x(),p.y());
}
void Area::setDirection(int x, int y, DIRECTION d, bool unidirection){

   if(x-1>=m_length||y-1>=m_width||x<1||y<1) return;
   if(m_roomInfo[x-1][y-1]->m_room.isEmpty()) return;
   if(m_roomInfo[x-1][y-1]->m_direction&d) return;
   QPoint p;
   if(!unidirection){
       if(!getLinkedPoint(QPoint(x,y),p,d)) return;
       if(getRoom(p).isEmpty()) return;
       setDirection(p.x(),p.y(),(DIRECTION)getNegativeDirection(d), true);
   }
    m_roomInfo[x-1][y-1]->m_direction|=d;

}
void Area::setDirection(const QPoint &p, DIRECTION d, bool unidirection){
    setDirection(p.x(),p.y(),d,unidirection);
}
void Area::delDirection(int x, int y, DIRECTION d, bool unidirection){
    if(x<1||y<1||x>m_length||y>m_width) return;
    QPoint p;
    if(!unidirection){
        if(!getLinkedPoint(QPoint(x,y),p,d)) return;
        delDirection(p.x(),p.y(),(DIRECTION)getNegativeDirection(d), true);
    }
     m_roomInfo[x-1][y-1]->m_direction^=d;
}
void Area::delDirection(const QPoint &p, DIRECTION d, bool unidirection){
    delDirection(p.x(),p.y(),d,unidirection);
}

int Area::getDirection(const QPoint &p) const{
    return getDirection(p.x(),p.y());
}
int Area::getDirection(int x, int y) const{
    if(x-1>=m_length||y-1>=m_width) return 0;
    if(x<0||y<0) return 0;
    return m_roomInfo[x-1][y-1]->m_direction;
}
bool Area::checkDirection(const QPoint &p,int d){
    int dirs=getDirection(p);
    return dirs&d;
}

int Area::getNegativeDirection(DIRECTION d) const{
  //  if(d==1||d==4||d==16||d==64) return d<<1;
   // else return d>>1;
    switch (d) {
    case E:
        return W;
        break;
    case W:return E;
    case N:return S;
    case S:return N;
    case SE:return NW;
    case NW:return SE;
    case SW:return NE;
    case NE:return SW;
    default:
        break;
    }
    return 0;
}
bool Area::getLinkedPoint(QPoint now, QPoint &linked, int d){
    int x=0,y=0;
    switch (d) {
    case E:
        x++;
        break;
    case W:
        x--;break;
    case N:
        y--;break;
    case S:
        y++;break;
    case SE:
        x++;y++;break;
    case NE:
        x++;y--;break;
    case SW:
        x--;y++;break;
    case NW:
        x--;y--;break;
    default:
        break;
    }
    x=now.x()+x;
    y=now.y()+y;
    if(x<1||x>m_length) return false;
    if(y<1||y>m_width) return false;
    linked.setX(x);
    linked.setY(y);
    return true;
}
void Area::delRoom(const QPoint &pos){
    delRoom(pos.x(),pos.y());
}

void Area::setObjects(int x,int y, const QString &s)
{
    if(x-1>=m_length||y-1>=m_width) return;
    m_roomInfo[x-1][y-1]->m_objs=s;
}

void Area::addObject(int x, int y, const QString &s)
{
    if(x-1>=m_length||y-1>=m_width) return;
    if(getObjects(x,y).isEmpty()){
        m_roomInfo[x-1][y-1]->m_objs=s;
    }
    else m_roomInfo[x-1][y-1]->m_objs+=QString("|%s").arg(s);
}

QStringList Area::getObjects(int x, int y) const
{
    if(x-1>=m_length||y-1>=m_width) return QStringList();
    QString str= m_roomInfo[x-1][y-1]->m_objs;
    if(str.isEmpty()) return QStringList();
    return str.split("|");
}
void Area::delRoom(int x, int y){
    if(x>m_length||y>m_width||x<1||y<1) return;
    m_roomInfo[x-1][y-1]->m_room="";
    m_roomInfo[x-1][y-1]->m_direction=0;
}
bool Area::Load(const QString &fileName){

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"area.load() can't open file:"<<fileName;
        return false;
    }
    QDataStream ar(&file);
    Serialize(ar,LOAD);
    m_fileName=fileName;
    m_fileName.replace(QDir::currentPath(),".");
    areaList.insert(m_areaName,this);
    return true;
}

bool Area::save(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"Save() can't open file:"<<fileName;
        return false;
    }
    m_fileName=fileName;
    m_fileName.replace(QDir::currentPath(),".");
    QDataStream ar(&file);
    Serialize(ar,SAVE);
    file.close();
    return true;
}
QString Area::getDirName(int d){
    QString str;
    switch(d){
    case E:
        str="东";
        break;
    case W:
        str="西";
        break;
    case N:
        str="北";
        break;
    case S:
        str="南";
        break;
    case SE:
        str="东南";
        break;
    case SW:
        str="西南";
        break;
    case NE:
        str="东北";
        break;
    case NW:
        str="西北";
        break;
    }
    return str;
}


RoomInfo::RoomInfo()
    :m_centerFlag("")
    ,m_room("")
    ,m_direction(0)
    ,m_areaLinkDir(0)
    ,m_areaLinkAddr("")
{

}

void RoomInfo::reset(){
    m_centerFlag="";
    m_room="";
    m_direction=0;
    m_areaLinkDir=0;
    m_areaLinkAddr="";
    m_XDirName="";
    m_YDirName="";
}
