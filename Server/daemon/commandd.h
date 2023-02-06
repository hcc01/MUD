#ifndef COMMANDD_H
#define COMMANDD_H

#include"./cuser.h"
#include<QMap>

#define cmdD CommandD::instance()

class CommandD
{
public:
    static CommandD& instance(){
        static CommandD cmdd;
        return cmdd;
    }

    void doCommand(CUser* user, const QString& msg);
private:
    CommandD();
    ~CommandD();
    void initCmd();//所有指令在这里添加
private:
    QMap<QString,void (*)(CUser*,const QString&)> *m_cmds;
};

#endif // COMMANDD_H
