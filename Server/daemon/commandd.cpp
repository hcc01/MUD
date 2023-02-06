#include "commandd.h"
#include"./Object/Object.h"
#include"./Object/Area.h"
#include"./Object/room.h"
#include"messaged.h"
#include <processthreadsapi.h>
CommandD::CommandD()
{
    m_cmds=new QMap<QString,void (*)(CUser*,const QString&)>;
    initCmd();
}

CommandD::~CommandD()
{
    delete m_cmds;
}

void CommandD::initCmd()
{
    m_cmds->insert("go",[](CUser* user,const QString& arg){
        if(arg.isEmpty()){
            MessageD::tellUser(user,"你要往哪走？");
            return;
        }
        int d=arg.toInt();
        Room* room=(Room*)user->getEnvironment();
        if(!room){
            MessageD::tellUser(user,"这里无法移动。");
            return;
        }
        int x,y;
        Area* area=room->getRoomPos(x,y);
        if(area->checkDirection(QPoint(x,y),d)){
            QPoint p;
            if(area->getLinkedPoint(QPoint(x,y),p,d)){
                user->moveTo(p.x(),p.y(),"");
                user->heartBeat(true);
                return;
            }
        }
        MessageD::tellUser(user,"这个方向没路。");
    });
}

void CommandD::doCommand(CUser *user, const QString &msg)
{
    qDebug()<<"command thead id:"<<GetCurrentThreadId();
    int n=msg.indexOf(" ");
    QString cmd,arg;
    void(*f)(CUser*, const QString&);
    if(n){
        cmd=msg.left(n);
        arg=msg.mid(n+1);
    }
    else {
        cmd=msg;
        arg="";
    }
    f=m_cmds->value(cmd);
    if(!f){
        MessageD::tellUser(user,"无效的指令。");
        return;
    }
    f(user,arg);
}
