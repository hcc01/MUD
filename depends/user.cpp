#include "user.h"
CELLUser::CELLUser():
    _clientSocket(nullptr),
    m_bLinked(false)
{
}

CELLUser::~CELLUser()
{
}

void CELLUser::sendMsg(const QJsonObject &msg)
{
    if(!m_bLinked) return ;
    _clientSocket->SendData(msg);
}
