#include "Object.h"
#include<QDebug>
#include<QApplication>
#include<QFile>
#include<QDir>
#include<QIODevice>
#include"Area.h"
#include"Room.h"
#include <processthreadsapi.h>
REGISTER_CLASS(Object);
Object::Object(QObject *parent) : QObject(parent)
  ,m_className("Object")
  ,m_Environment(nullptr)
  ,m_copyName("")
  ,m_fileName("")
{
     qDebug()<<"Object() thead id:"<<GetCurrentProcessId();
    objectCount++;
    setType(OBJECT);
    m_bIsClone=false;
//    m_Timer=new QTimer(this);
//    //m_Timer->start(60000*30);//30分钟进行一次清理
//    m_Timer->start(1000);
//    m_Timer->setInterval(1000);
//    connect(m_Timer,SIGNAL(timeout()),this,SLOT(heartBeat()));
//    m_Timer->start();
//    startTimer(1000);

}

Object::~Object()
{

}

quint64 Object::objectCount=0;
void Object::Serialize(QDataStream &ar, SERIA_TYPE st){
    QByteArray json;
    switch(st){
    case SAVE:
        json=QJsonDocument(m_Data).toJson();
        ar<<m_className<<m_fileName<<json;
        break;
    case LOAD:
        ar>>m_className>>m_fileName>>json;

        m_Data=((QJsonDocument::fromJson(json).object()));
        break;
    case GET_CLASS:
        ar>>m_className;
        break;
    }
}

Object* Object::CreateObject(const QString &str){
    QString className=getClassName(str);
    qDebug()<<"className"<<className;
     Object* ob=static_cast<Object *>(DynObjectFactory::CreateObject( className));
     if(!ob) return 0;
     ob->setCloneName(QString("%1#%2").arg(str).arg(objectCount));
     ob->m_bIsClone=true;
     return ob;
}
void Object::set(const QString & k, const QString &v){
    m_Data.insert(k,v);
}
void Object::set(const QString & k, const QJsonObject &v){
    m_Data.insert(k,v);
}
void Object::set(const QString & k, int v){
    m_Data.insert(k,v);
}


void Object::del(const QString &key){
    m_Data.remove(key);
}


void Object::set_temp(const QString & k, const QString &v){
    m_tempData.insert(k,v);
}
void Object::set_temp(const QString & k, const QJsonObject &v){
    m_tempData.insert(k,v);
}
void Object::set_temp(const QString & k, int v){
    m_tempData.insert(k,v);
}


void Object::del_temp(const QString &key)
{
    m_tempData.remove(key);
}
QJsonValue Object::get_temp(const QString & k)const{
    if(m_tempData.isEmpty()){
        return 0;
    }
    return m_tempData.value(k);
}

QJsonValue Object::get(const QString & k)const{

    if(m_Data.isEmpty()){
        return 0;
    }
    return m_Data.value(k);
}
void Object::Save(){
  Save(m_fileName);
}

void Object::Save(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)){
        qDebug()<<"Save() can't open file:"<<fileName;
        return;
    }
    m_fileName=fileName;
    m_fileName.replace(QDir::currentPath(),".");
    QDataStream ar(&file);
    Serialize(ar,SAVE);
    file.close();
}
bool Object::Load(const QString &path,bool JustGetClassName){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Load(path) can't open file:"<<path;
        return false;
    }
    QDataStream ar(&file);
    if(JustGetClassName) Serialize(ar,GET_CLASS);
    else Serialize(ar,LOAD);
    file.close();
    return true;
}
bool Object::Load(){
    QFile file(m_fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Load() can't open file:"<<m_fileName;
        return false;
    }
    QDataStream ar(&file);
    Serialize(ar,LOAD);
    file.close();
    return true;
}
bool Object::AcceptObject(Object *, const QString &){
    return true;
}

void Object::setClass(const QString &className)
{
    if(!DynObjectFactory::ClassNames.contains(className)){
        qDebug()<<"wrong class name"<<className;
        return;
    }
    m_className=className;
}

QString Object::getClassName(const QString &file)
{
    static Object ob;
    if(!ob.Load(file)) {
        qDebug()<<"无法读取文件："<<file;
        return "";
    }
    return ob.className();
}

//void Object::AddAction(const QString & v ,int t){
//    actions.insert(v,t);
//}
void Object::CleanUp(){
    if(!m_bIsClone) return;//非自动创建的物件需要手动清理
    if(get_temp("no_cleanup").toInt()) return;
    if(getEnvironment()) return;//处理环境中，不清理。
    if(!m_Contents.isEmpty()&&type()==ROOM){
        QMap<QString, Object*>::iterator it = m_Contents.begin();
                while (it != m_Contents.end())
                {
                    if(it.value()->type()==USER)//有玩家在里面，不清理
                       return;
                    else
                        ++it;
                }
    }
    Destruct();
}


//void Object::doAction(int &, Object *, const QString  &){

//}

//int Object::FindAction(const QString &&verb){
//    return actions.value(verb);
//}
//void Object::Command(QByteArray v, QByteArray a){
//   ::Command((void*)this,v,a);
//}
bool Object::moveTo(Object *ob){
    Object* env=ob->getEnvironment();
    if(!ob->AcceptObject(this,"")) return false;
    this->m_Environment=ob;
    ob->addObject(this);
    if(env){
        env->removeObject(this);
    }
    return true;
}
bool Object::moveTo(int x,int y, const QString& name){//e.g. move(5,2,"taohuagu");
    qDebug()<<"ob id:"<<GetCurrentThreadId();
    Area *area;
    if(!name.isEmpty()) area=Area::getArea(name);
    else area=((Room*)getEnvironment())->getArea();
    if(!area) return false;
    Object* room=area->getRoomOb(x,y);
    if(!room) return false;
    return moveTo(room);
  //  name=area->getRoom(x,y);

}



void Object::Destruct(){
    if(type()==USER) return;
    if(m_Environment){
        m_Environment->removeObject(this);
//        if(m_bIsEquip){

//        }
    }
    for(int i=0;i<m_Contents.count();i++){
        m_Contents.values().at(i)->Destruct();
    }
    qDebug()<<" clearing."<<getCloneName();
    delete this;
    qDebug()<<" object is cleared.";
}
//void Object::sendMsg(const QString &,int msgType){
//}
//void Object::sendMsg(const QString &, int msgType){
//}
//Object* Object::getObject(const QString  &name){//保留用ID找物件
//   if(name.indexOf("#")>0) return m_Contents.value(name);
//   int n=0;
//   n=name.indexOf(" ");
//   if(n>0){
//       bool ok;
//       QByteArray s;
//       s=name;
//       name=s.left(n);
//       s=s.mid(n+1);
//      n= s.toInt(&ok);
//      if(!ok) return NULL;
//   }
//   else n=1;
//   QMap<QString, Object*>::iterator it = m_Contents.begin();
//   while (it != m_Contents.end())
//   {
//       if(it.value()->get("id").toString()==name){
//           n--;
//           if(!n) return it.value();
//          }
//   }
//   return NULL;
//}
