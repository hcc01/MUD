#include "Role.h"
#include"../daemon/timerd.h"
#include <processthreadsapi.h>
Role::Role():m_bWake(true)
{
    TimerD::instance().startHeartBeat(this);
    qDebug()<<"Role() thead id:"<<GetCurrentProcessId();
}

void Role::heartBeat(bool show)
{
    static int n=0;n++;
    if(n==1||show) qDebug()<<QString("heartBeat线程ID：%1，%2心跳中，第%3次。").arg(GetCurrentProcessId()).arg(name()).arg(n);
}
