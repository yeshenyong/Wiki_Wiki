#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "object.h"
#include "classinfo.h"
#include <iostream>
using std::string;
class Factory
{
private:
    Factory(){}
public:
    virtual ~Factory();
    static Factory* GetInstance();
    static bool Register(ClassInfo* ci);    // 注册传入一个classInfo（类信息），将这个类的信息注册到映射当中
    static Object* CreateObject(string name);   //工厂生产对象的接口
};

#endif


