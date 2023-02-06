#include "cloginmanager.h"
#include"cdatabasemanage.h"
#include"fucmap.h"
#include<QFile>
#include"./daemon/messaged.h"
CLoginManager::CLoginManager(QObject *parent) : QObject(parent)
{

}

void CLoginManager::doLogin(CELLClient *pClient, const QString &id, const QString &passwd)
{
    netmsg_LoginR lr;
    CDatabaseManage &dbManager=CDatabaseManage::Instance();
    if(!dbManager.isOpen()) {
       CELLLog::Info("doLogin error: db not open.");
       lr.result=DB_ERROR;
       pClient->SendData(&lr);
       return ;
    }
    QSqlQuery query(dbManager.database());
    QString sql(QString("select * from sys_employee_login where name='%1' and password='%2'").arg(id).arg(passwd));
    if(!query.exec(sql)){
        CELLLog::Info("Login error: %s",query.lastError().text());
        lr.result=DB_ERROR;
        pClient->SendData(&lr);
        return ;
    }
    //QSqlRecord rcd=query.record();
    if(query.next()) {                              //密码验证成功
       lr.result=LOGIN_SUCCESSED;
       CUser* user=new CUser;
       user->linktoClient(pClient);
       if(!user->Load("./user/"+id+".o")){//创建新用户
           user->setID(id);
           MessageD::tellUser(user,"你的档案已损坏，请重新创建档案。请输入你的角色大名：");
           user->setReply(FUCMAP::NEW_USER_NAME);
           FuncMap::instance().m_fucMap.insert(FUCMAP::NEW_USER_NAME, &CLoginManager::onNewUserName);

       }
//       else if(_userMap.contains(user->id())) {//断线重连或重复登录的，暂时不处理
//           CUser* oldUser=_userMap.value(user->id());
//           CELLClient* oldClient=oldUser->client();
//           if(oldClient&&oldClient!=pClient){//其它地方登录，踢出原来连接
//               QJsonObject jsonCMD;
//               jsonCMD["cmd"]=CMD_LOGOUT;
//               jsonCMD["data"]=QString("有人从%1处登录了该账号，如果不是本人操作，请及时更改密码。").arg(pClient->IP());
//               oldClient->SendData(jsonCMD);
//               oldClient->setUser(nullptr);
//               oldUser->linktoClient(pClient);
//           }
//           else oldUser->linktoClient(pClient);//重新连接。网络断线时未清理USER（见onNectError），所以直接连接。
//       }
       else{
           user->movetoStartRoom();
           addUser(user);
       }
        pClient->SendData(&lr);             //传回登录成功的消息

    }
    lr.result=LOGIN_FAIL;
    pClient->SendData(&lr);

}

void CLoginManager::doReg(CELLClient *pClient,RegMsg regInfo)
{
    bool result;QString msg;
    CDatabaseManage &dbManager=CDatabaseManage::Instance();
    if(!dbManager.isOpen()) {
       CELLLog::Info("doLogin error: db not open.");
       pClient->SendData(RegResultMsg(false,"数据库错误").msgInfo());
       return ;
    }
    QSqlQuery query(dbManager.database());
    QString sql(QString("select * from sys_employee_login where name='%1'").arg(regInfo.id()));
    if(!query.exec(sql)){
        CELLLog::Info("Login error: %s",query.lastError().text());
        pClient->SendData(RegResultMsg(false,"数据库错误").msgInfo());
        return ;
    }
    if(query.next()){
        pClient->SendData(RegResultMsg(false,"账号已经存在，请直接登录").msgInfo());
        return ;
    }
    //开始注册
     sql=(QString("insert into sys_employee_login (name,password) values('%1','%2')").arg(regInfo.id()).arg(regInfo.password()));
    if(!query.exec(sql)){
        qDebug()<<("Reg error: %s"+query.lastError().text());
        pClient->SendData(RegResultMsg(false,"数据库操作错误").msgInfo());
        return ;
    }
//    QFile file(QString("./user/%1.o").arg(regInfo.id()));
//    if(!file.open((QIODevice::ReadWrite))){
//        qDebug()<<"创建文件失败"<<file.fileName();
//    }
    CUser* user=new CUser(regInfo.id(),regInfo.name(),regInfo.gender());
    user->Save(QString("./user/%1.o").arg(regInfo.id()));
    user->linktoClient(pClient);
    pClient->SendData(RegResultMsg(true,"").msgInfo());
    initNewUser(user);

}

void CLoginManager::addUser(CUser* user)
{
    if(!user) return;
    _users.append(user);
    _userMap[user->id()]=user;
}

void CLoginManager::removeUser(CUser *user)
{
    if(!user) return;
    _users.removeOne(user);
    _userMap.remove(user->id());
}

CUser *CLoginManager::findUser(const QString &id)
{
    if(!_userMap.contains(id)) return nullptr;
    return _userMap.value(id);
}

void CLoginManager::onNewUserName(CUser *user, const QString &str, void *lm)
{
    user->setName(str);
    MessageD::tellUser(user,"你的档案已损坏，请重新创建档案。请选择你的性别：1、男性 2、女性（选序号)");
    user->setReply(FUCMAP::NEW_USER_NAME);
    FuncMap::instance().m_fucMap.insert(FUCMAP::NEW_USER_GENDER,&CLoginManager::onNewUserGender);
}

void CLoginManager::onNewUserGender(CUser *user, const QString &str, void *lm)
{
    int gender=str.toInt();
    if(gender!=1&&gender!=2){
        MessageD::tellUser(user,"对不起，输入错误。请选择你的性别：1、男性 2、女性（选序号)");
        FuncMap::instance().m_fucMap.insert(FUCMAP::NEW_USER_GENDER,&CLoginManager::onNewUserGender);
        return;
    }
    gender-=1;
    user->setGender(gender);
    ((CLoginManager*)lm)-> initNewUser(user);
}

void CLoginManager::initNewUser(CUser *user)
{
    user->setStartRoom("wangyougu",4,3);
    user->movetoStartRoom();
    user->Save(QString("./user/%1.o").arg(user->id()));
    addUser(user);
}
