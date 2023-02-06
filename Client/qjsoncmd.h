#ifndef QJSONCMD_H
#define QJSONCMD_H
#include<QVariant>
#include<QJsonObject>
#include<QJsonArray>

#include"../../depends/MessageHeader.h"
enum JSON_CDM{
    JC_REPLY,
    JC_WORKFLOW,
    JC_NOTICE,
    JC_REG_INFO,
    JC_REG_RETURN,
    JC_ENVIRONMENT,
    JC_COMMAND,
};
class JsonMsg
{
public:
    JsonMsg(const QJsonObject&);
    JsonMsg(){

    }
    virtual ~JsonMsg(){}
    void setType(JSON_CDM type){m_jsMsg.insert("type",type);}
    int type()const{return m_jsMsg.value("type").toInt();}
    QJsonObject  msgInfo()const{return m_jsMsg;}
    void set(const QString& key,const QJsonValue& value){
        m_jsMsg.insert(key,value);
    }
    QJsonValue get(const QString& key){ return m_jsMsg.value(key);}
private:
    QJsonObject m_jsMsg;

};

class RegMsg:public JsonMsg
{
public:
    RegMsg(const QJsonObject& regInfo):JsonMsg(regInfo){setType(JC_REG_INFO); }
    RegMsg(const QString& id,const QString& password,const QString& name,int gender);
    QString id(){return get("id").toString(); }
    QString password(){return get("password").toString(); }
    QString name(){return get("name").toString(); }
    int gender(){return get("gender").toInt();}
private:
};
class RegResultMsg:public JsonMsg
{
public:
    RegResultMsg(){setType(JC_REG_RETURN);}
    RegResultMsg(const QJsonObject& msg):JsonMsg(msg){setType(JC_REG_RETURN);}
    RegResultMsg(bool regSuccess,const QString& msg){setType(JC_REG_RETURN);set("regSuccess",regSuccess);set("msg",msg);}
    bool isSuccess(){return get("regSuccess").toBool();}
    QString msg(){return get("msg").toString();}
};

class EnvirontmentMsg:public JsonMsg
{
public:
    EnvirontmentMsg(const QJsonObject& msg):JsonMsg(msg){setType(JC_ENVIRONMENT);hasOb=false;}
    EnvirontmentMsg(){setType(JC_ENVIRONMENT);}
    void setAreaMsg(const QString&areaName,int x,int y){set("area",QString("%1@%2,%3").arg(areaName).arg(x).arg(y));}
    void getAreaMsg(QString&areaName,int&x,int&y);
    void setRoomFile(const QString&file){set("room",file);}
    void setObjectMsg(const QString&msg){set("object",msg);}
    void addObjectMsg(const QString& obCloneName,int obType);
    QMap<QString,int>getObjectsInfo();
    QString areaMsg(){return get("area").toString();}
    QString roomMsg(){return get("room").toString();}
    QString objectMsg(){return get("object").toString();}
private:
    bool hasOb;
};


class NoticeMsg:public JsonMsg
{
public:
    NoticeMsg(const QString& msg){
        setType(JC_NOTICE);
        set("msg",msg);
    }
    NoticeMsg(const QJsonObject& data):JsonMsg(data)
    {
        setType(JC_NOTICE);
    }
    QString msg(){return get("msg").toString();}
};

class ReplyMsg:public JsonMsg//客户端输入需要回复的信息时
{
public:
    ReplyMsg(const QString& msg){
        setType(JC_REPLY);
        set("msg",msg);
    }
    ReplyMsg(const QJsonObject& data):JsonMsg(data)
    {
        setType(JC_REPLY);
    }
    QString msg(){return get("msg").toString();}
};

class CommandMsg:public JsonMsg
{
public:
    CommandMsg(const QString& cmd){
        setType(JC_COMMAND);
        set("cmd",cmd);
    }
    CommandMsg(const QJsonObject& data):JsonMsg(data)
    {
        setType(JC_COMMAND);
    }
    QString Cmd(){return get("cmd").toString();}
};

#endif // QJSONCMD_H
