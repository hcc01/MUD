#ifndef TIMERD_H
#define TIMERD_H

#include <QObject>
#include"../Object/Role.h"
class TimerD:public QObject
{
    Q_OBJECT
public:
    TimerD();
    static TimerD& instance(){
        static TimerD t;
        return t;
    }
    virtual void timerEvent(QTimerEvent *event);
    void startHeartBeat(Role*);
private:
    QList<Role*> m_roles;
    int m_heartBeatID;
};

#endif // TIMERD_H
