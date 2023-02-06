#include "cserer.h"
#include"QSqlError"
#include<QDebug>
#include"cdatabasemanage.h"
#include<QDateTime>
#include"fucmap.h"
#include"./daemon/commandd.h"
#include <processthreadsapi.h>
CServer::CServer()
{
     qDebug()<<"CServer() thead id:"<<GetCurrentProcessId();
}

void CServer::OnNetJoin(CELLClient *pClient)
{
    EasyTcpServer::OnNetJoin(pClient);
}

void CServer::OnNetLeave(CELLClient *pClient)
{
    EasyTcpServer::OnNetLeave(pClient);
    if(pClient->isLogined()) _lgManager.removeUser((CUser*)pClient->getUser());
}

void CServer::OnNetError(CELLClient *pClient)
{
    EasyTcpServer::OnNetLeave(pClient);
    //断线时应当给一定时间让用户进行重新连接，以免用户数据丢失（用在游戏时）
}

void CServer::OnNetMsg(CELLServer *pServer, CELLClient *pClient, netmsg_DataHeader *header)
{
    EasyTcpServer::OnNetMsg(pServer,pClient,header);
    auto cmd=header->cmd;
    switch(cmd){
        case CMD_LOGIN:
        {
            netmsg_Login* msgLg=(netmsg_Login*)header;
            _lgManager.doLogin(pClient,msgLg->userName,msgLg->PassWord);
          //  if(user) init(user);//这边一弄，DOLOGIN里面的发送登录结果的消息客户端就收不到，不知道 为什么。
        }
        break;
        case CMD_C2S_HEART:
        {
            pClient->resetDTHeart();
            netmsg_s2c_Heart ret;
            pClient->SendData(&ret);
        }
        break;
//    case CMD_INIT:
//    {
////        init(pClient);
//    }
//        break;
    case CMD_JSON_CMD:
    {

        JsonMsg jm(CELLReadStream(header).getJsonData());
        onJsonCMD(pClient,jm);
    }
        break;
    default:
        CELLLog::Info("recv <ip=%s> undefine msgType,dataLen：%d\n", pClient->IP(), header->dataLength);
    }
}

void CServer::onJsonCMD(CELLClient *pClient, const JsonMsg &jm)
{
    int type=jm.type();
    CUser* user=(CUser*)pClient->getUser();
    if(user&&!user->isWake())//昏迷状态，屏蔽任何指令
        return;
    switch (type) {
        case JC_REG_INFO:
    {
        _lgManager.doReg(pClient,RegMsg(jm.msgInfo()));
    }
        break;
    case JC_REPLY:
    {
        int replyTo=user->replyTo();
        ReplyMsg rMsg(jm.msgInfo());
        FuncMap::instance().m_fucMap.value(replyTo)(user,rMsg.msg(),(void*)&_lgManager);
    }
        break;
    case JC_COMMAND:
    {
        CommandMsg cmdMsg(jm.msgInfo());
        cmdD.doCommand(user,cmdMsg.Cmd());
    }
        break;
    default:
        qDebug()<<"收到未知的JSON数据。"<<jm.msgInfo();
        break;
    }
}
