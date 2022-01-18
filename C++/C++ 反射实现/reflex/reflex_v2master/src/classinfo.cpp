#include "../include/classinfo.h"
#include "../include/factory.h"

ClassInfo::ClassInfo(const std::string className, ObjectConstructorFn ctor):m_className(className), m_objectConstructor(ctor)
{
    std::cout << "Register\n";
    Factory::Register(this);     //classInfo 的构造函数是传入类名和类对应的new函数然后自动注册进map 中
}

