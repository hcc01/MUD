#include "qjsoncmd.h"
JsonMsg::JsonMsg(const QJsonObject &msgInfo)
{
    m_jsMsg=msgInfo;
}

RegMsg::RegMsg(const QString &id, const QString &password, const QString &name, int gender)
{
    set("id",id);
    set("password",password);
    set ("name",name);
    set("gender",gender);
    setType(JC_REG_INFO);
}

void EnvirontmentMsg::getAreaMsg(QString &areaName, int &x, int &y)
{
    QString str=msgInfo().value("area").toString();
    int n=str.indexOf("@");
    if(n<1){
        qDebug()<<"JC_ENVIRONMENT error:"<<str;
        return;
    }
    areaName=str.left(n);
    str=str.mid(n+1);
    n=str.indexOf(",");
    x=str.left(n).toInt();
    y=str.mid(n+1).toInt();
}

void EnvirontmentMsg::addObjectMsg(const QString &obCloneName, int obType)
{
    if(!hasOb) {
        msgInfo()["object"]=QString("%1@%2").arg(obCloneName).arg(obType);
        hasOb=true;
    }
    else msgInfo()["object"]=msgInfo().value("object").toString()+QString("|%1@%2").arg(obCloneName).arg(obType);
}

QMap<QString, int> EnvirontmentMsg::getObjectsInfo()
{
    QMap<QString, int> obMap;
    QString str=msgInfo().value("object").toString();
    if(str.isEmpty()) return obMap;
    QStringList objList=str.split("|");
    for(int i=0;i<objList.size();i++){
        str=objList.at(i);
        int n=str.indexOf("@");
        obMap.insert(str.left(n),str.mid(n+1).toInt());
    }
    return obMap;
}
