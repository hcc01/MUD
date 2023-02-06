#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include<QObject>
#include"dynbase.h"//动态创建物件
#include<QMap>
#include<QJsonDocument>
#include<QJsonObject>
#include<QDataStream>
#include<QDebug>
class Room;
class Object : public QObject
{
    Q_OBJECT
//protected:
//    ~Object(){}
public:
    enum OBJ_TYPE{
        OBJECT,ROOM,NPC,USER,
    };
    explicit Object(QObject *parent = nullptr);
    ~Object();
    static quint64 objectCount;
    static Object* CreateObject(const QString& );    
    static QString getClassName(const QString& file);
 enum SERIA_TYPE{SAVE,LOAD,GET_CLASS};
    virtual void Save();
    virtual void Save(const QString& fileName);
    virtual bool Load(const QString& fileName, bool JustGetClassName=false);
    virtual bool Load();
    virtual void Serialize(QDataStream & ar,SERIA_TYPE st);
    virtual bool AcceptObject(Object*, const QString &);
    void setClass(const QString& className);
    QString className()const{return m_className;}
    void setType(OBJ_TYPE type){m_type=type;}
    OBJ_TYPE type()const{return m_type;}
    void setFileName(const QString& name){m_fileName=name;}
    QString getFileName()const{return m_fileName;}
    void set(const QString&,const QString&);
    void set(const QString&,int);
    void set(const QString&,const QJsonObject&);

    void del(const QString&);

    void set_temp(const QString&,const QString&);
    void set_temp(const QString&,int);
    void set_temp(const QString&,const QJsonObject&);

     void del_temp(const QString&);

    QJsonValue get(const QString& k)const;
    QJsonValue get_temp(const QString& k)const;
    QJsonObject getData()const{return m_Data;}
    void setData(const QJsonObject& data){m_Data=data;}

//    int FindAction(const QString&);
//    void Command(const QString&, const QString&);
    void setEnvironment(Object* e){m_Environment=e;}
    Object* getEnvironment(){return m_Environment;}
    void setCloneName(const QString& name){m_copyName=name;}
    QString getCloneName()const{if(type()==USER) return get("ID").toString();return m_copyName;}

    void removeObject(Object* ob){m_Contents.remove(ob->getCloneName());}
    Object* queryObject(QString& name);
    QMap<QString,Object*> getContents(){return m_Contents;}
    virtual bool moveTo(Object*);//设为虚函数，让不同物件有不同的移动效果
    virtual bool moveTo(int x, int y, const QString &areaName);
    virtual void addObject(Object* ob){m_Contents.insert(ob->getCloneName(),ob);}
    virtual void Destruct();
    virtual void CleanUp();
    void setName(const QString& name){set("name",name);}
    virtual QString name(){return get("name").toString();}
public slots:
signals:


private:
    QMap<QString,Object*> m_Contents;//内容物<物件识别号，物件>
    QString m_fileName;//记录文件名
    QString m_className;//记录类名
    QMap<QString,int> actions;//可执行的指令
    Object* m_Environment;//物件所处环境
    QString m_copyName;//物件识别号
    QJsonObject m_Data;//数据
    QJsonObject m_tempData;//临时数据，不保存

    OBJ_TYPE m_type;//物件类型，用于设置子类物件类型（房间、物品、食物、人物、装备等）
public:
    bool m_bIsClone;
};

#endif // OBJECT_H
