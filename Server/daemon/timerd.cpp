#include "timerd.h"
#include<QTimerEvent>
TimerD::TimerD()
{
    m_heartBeatID=startTimer(1000);
}

void TimerD::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==m_heartBeatID){
        for(int i=0;i<m_roles.count();i++) m_roles.at(i)->heartBeat();
    }
}

void TimerD::startHeartBeat(Role *r)
{
    m_roles.append(r);
}
