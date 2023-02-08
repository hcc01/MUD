#ifndef USER_H
#define USER_H
//用户类，用于登录后的用户信息管理。
#include"CELLClient.h"

class CELLUser
{
public:
    CELLUser();
    virtual ~CELLUser();
    virtual void OnDisconnect()=0;//断开连接事件,用以清理USER资源。
    void linktoClient(CELLClient* client){
        _clientSocket=client;
        client->setUser(this);
        m_bLinked=true;
    }
    CELLClient* client()const{
        return _clientSocket;
    }
    void sendMsg(const QJsonObject&);
private:
    CELLClient* _clientSocket;
    bool m_bLinked;
};

#endif // USER_H
