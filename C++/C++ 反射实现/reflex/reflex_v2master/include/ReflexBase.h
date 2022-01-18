#ifndef __REFLEX_BASH_H__
#define __REFLEX_BASH_H__
#include "classinfo.h"
#include "object.h"

// 类声明中添加 classInfo 属性和 CreateObject、GetClassInfo
#define DECLARE_CLASS(name) \   
    protected:  \    
        static ClassInfo ms_classInfo;  \    
    public: \   
        virtual ClassInfo* GetClassInfo() const;    \  
        static Object* CreateObject();

// 实现CreateObject 和 GetClassInfo 的两个方法
#define IMPLEMENT_CLASS_COMMON(name, func)  \    
    ClassInfo name::ms_classInfo((#name),   \ 
                    (ObjectConstructorFn) func);    \  
    ClassInfo *name::GetClassInfo() const   \ 
                    { return &name::ms_classInfo; }
    
// classInfo 属性初始化
#define IMPLMENT_CLASS(name)    \  
    IMPLEMENT_CLASS_COMMON(name, name::CreateObject)    \  
    Object* name::CreateObject()    \  
            { return new name; }

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
