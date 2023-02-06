#ifndef _DYN_BASE_H_
#define _DYN_BASE_H_

#include <map>
#include <string>
#include<QObject>
#include<QStringList>
using  namespace std;

typedef  void *(*CREATE_FUNC)();//定义一个函数指针

class DynObjectFactory
{
public:
     static QStringList ClassNames;
     static  void *CreateObject( const QString &name)
    {
        map<QString, CREATE_FUNC>::const_iterator it;
        it = mapCls_.find(name);
         if (it == mapCls_.end())
             return  0;
         else
             return it->second();  //func();

    }

     static  void Register( const QString &name, CREATE_FUNC func)
    {
        mapCls_[name] = func;
        ClassNames.append(name);
    }
public:
     static map<QString, CREATE_FUNC> mapCls_;
};

// g++
// __attribute ((weak))
__declspec(selectany) map<QString, CREATE_FUNC> DynObjectFactory::mapCls_;
__declspec(selectany) QStringList DynObjectFactory::ClassNames;
//头文件被包含多次，也只定义一次mapCls_;

class Register
{
public:
    Register( const QString &name, CREATE_FUNC func)
    {
        DynObjectFactory::Register(name, func);
    }
};

#define REGISTER_CLASS(class_name) \
class class_name##Register { \
public: \
     static  void* NewInstance() \
    { \
         return  new class_name(); \
    } \
private: \
     static Register reg_; \
}; \
Register class_name##Register::reg_(#class_name, class_name##Register::NewInstance)
//CircleRegister

#endif  // _DYN_BASE_H_
