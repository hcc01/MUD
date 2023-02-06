#ifndef ROLE_H
#define ROLE_H

#include"Object.h"
class Role:public Object
{
public:
    enum GENDER{
       MAN,WOMAN
    };
    Role();
    bool gender()const{return get("gender").toBool();}
    void setGender(bool g){set("gender",g);}
    void setWake(bool wake){m_bWake=wake;}
    bool isWake()const{return m_bWake;}
    void heartBeat(bool show=false);
private:
    bool m_bWake;
};

#endif // ROLE_H
