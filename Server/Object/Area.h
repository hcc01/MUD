#ifndef AERA_H
#define AERA_H
#include<QPoint>
#include<QObject>
#include<QDataStream>
#include<QMap>
#include<QColor>
#include"Object.h"
#include<QChar>

class RoomInfo{
public:
    enum LAND_TYPE{//地形
        LT_TREE,LT_STONE,LT_RIVER,LT_CLIFF
    };

    RoomInfo();
    void reset();
    QString m_centerFlag;//显示标识
    QString m_room;//用于调用房间
    QString m_objs;//用于调用物件，物件调用后放入房间；保存格式：文件名1@数量1@机率1(单位：ppm)|文件名2@数量2@机率2|……
    int m_direction;
    int m_areaLinkDir;//区域接口方向，如果有，只设置1个方向
    QString m_areaLinkAddr;//区域入口位置
    QString m_XDirName;//自定义方向的名称，如：进入、出来、上、下等。
    QString m_YDirName;
    int m_landType;//地形（树林、石头、河流、悬崖等；树木可以被砍伐，砍伐后可以生成房间
};

class Area:public QObject
{
    Q_OBJECT
public:
    Area();
    ~Area();
#define MAX_LEN 100
    enum DIRECTION{E=1,W=2,N=4,S=8,SE=16,NW=32,SW=64,NE=128,X=256,Y=512};//X,Y为两个自定义方向，方向名称需要在AreaRoomInfo中保存
    enum SERIA_TYPE{SAVE,LOAD};
    static QMap<QString,Area*> areaList;
    static Area * getArea(QString areaName);
    void Serialize(QDataStream & ar,SERIA_TYPE st);
    void setRoom(int,int,const QString&);
    void delRoom(const QPoint& pos);
    void setObjects(int x, int y, const QString&);
    void addObject(int x,int y,const QString&);
    QStringList getObjects(int x,int y)const;
    void delRoom(int x,int y);
    void setRoomOb(Object* room,int x,int y);
    Object* getRoomOb(int x,int y);
    void delDirection(int x,int y,DIRECTION d,bool unidirection=false);
    void delDirection(const QPoint& p,DIRECTION d,bool unidirection=false);
    QString getRoom(int,int);
    QString getRoom(QPoint);
    void setDirection(int,int,DIRECTION,bool unidirection=false);
    void setDirection(const QPoint&,DIRECTION,bool unidirection=false);
    int getDirection(int,int)const;
    int getDirection(const QPoint&)const;
    bool checkDirection(const QPoint &, int d);
    int getLength(){return m_length;}
    int getWidth(){return m_width;}
    void setSize(int x,int y);
    bool checkSize(int x,int y){ return x>0&&y>0&&x<MAX_LEN && y < MAX_LEN ;}
    int getNegativeDirection(DIRECTION d)const;
    bool getLinkedPoint(QPoint now, QPoint& linked, int d);
    void setAreaName(const QString& name){m_areaName=name;}
    QString getAreaName(){return m_areaName;}
    QString getFileName(){return m_fileName;}
    bool Load(const QString&fileName);
    bool save(const QString &fileName);
    QString getDirName(int d);
    RoomInfo* getRoomInfo(int x,int y){
        return m_roomInfo[checkLen(x,true)][checkLen(y,false)];
    }
    void setRoomInfo(RoomInfo* af,int x,int y){
        m_roomInfo[checkLen(x,true)][checkLen(y,false)]=af;
    }
    void setCenterFlag(const QString& flag,int x,int y){getRoomInfo(x,y)->m_centerFlag=flag;}
    QString getCenterFlag(int x,int y){return getRoomInfo(x,y)->m_centerFlag; }
    int checkLen(int x,bool width){if(x<1) x=1;int l=(width?m_width:m_length);if(x>l) x=l;return x;}
private:
    int m_length;
    int m_width;
    RoomInfo* m_roomInfo[MAX_LEN][MAX_LEN];//布局信息，存档内容
   // int m_Direction[MAX_LEN][MAX_LEN];
    Object* m_RoomOb[MAX_LEN][MAX_LEN];
    QString m_areaName;
    QString m_fileName;
};

#endif // AERA_H
