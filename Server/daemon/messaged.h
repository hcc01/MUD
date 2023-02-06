#ifndef MESSAGED_H
#define MESSAGED_H

#include"../cuser.h"
#include"../../src/Client/qjsoncmd.h"
class MessageD
{
public:
    MessageD();
    static void sendEnvMsg(CUser* user);
    static void tellUser(CUser* user,const QString& msg);
};

#endif // MESSAGED_H
