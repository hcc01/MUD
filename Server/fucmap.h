#ifndef FUCMAP_H
#define FUCMAP_H
#include"cuser.h"
#include<QMap>
namespace FUCMAP {
    enum FUCKey{
        NEW_USER_NAME,NEW_USER_GENDER,
    };
}

class FuncMap
{
private:
    FuncMap();
public:
    static FuncMap& instance(){
        static FuncMap dm;
        return dm;
    }
//private:
    QMap<int,void(*)(CUser*,const QString&,void*)> m_fucMap;
};

#endif // FUCMAP_H
