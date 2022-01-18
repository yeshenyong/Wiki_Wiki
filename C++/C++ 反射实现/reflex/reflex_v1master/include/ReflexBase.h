#ifndef __REFLEX_BASH_H__
#define __REFLEX_BASH_H__
#include "Reflex.h"

class B : public Object
{
protected:
    /* data */
    static ClassInfo ms_classinfo;
public:
    B(/* args */) { std::cout << std::hex << (long)this << " B constructor" << std::endl; }
    ~B() { std::cout << std::hex << (long)this << " B destructor" << std::endl; }
    virtual ClassInfo* getClassInfo() const { return &ms_classinfo; }
    static Object* CreateObject() { return new B; }
};
ClassInfo B::ms_classinfo("B", B::CreateObject);

class C : public Object
{
    DECLARE_CLASS(C);
public:
    C() { std::cout << " C constructor" << std::endl; }
    ~C() { std::cout << " C destructor" << std::endl; }
};
IMPLMENT_CLASS(C);

#endif
