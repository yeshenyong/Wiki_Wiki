#ifndef __CLASSINFO_H__
#define __CLASSINFO_H__
#include "object.h"
#include <iostream>

typedef Object* (*ObjectConstructorFn)(void);

class ClassInfo
{
public:
    /* data */
    std::string m_className;
    ObjectConstructorFn m_objectConstructor;
public:
    ClassInfo(const std::string className, ObjectConstructorFn ctor);
    virtual ~ClassInfo() {}
    Object* CreateObject() const { return m_objectConstructor ? (*m_objectConstructor)() : 0; }
    bool isDynamic() const { return m_objectConstructor != nullptr; }
    ObjectConstructorFn getConstructor() const { return m_objectConstructor; }
};
#endif
