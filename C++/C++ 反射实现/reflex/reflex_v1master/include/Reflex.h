#ifndef __REFLEX_H__
#define __REFLEX_H__
#include <iostream>
using std::string;
class ClassInfo;
class Object;
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

class Object
{
private:
    /* data */
public:
    Object(){}
    virtual ~Object(){}
};

class Factory
{
private:
    /* data */
    Factory(){}
public:
    virtual ~Factory(){}
    static Factory* GetInstance();
    static bool Register(ClassInfo* ci);    // 注册传入一个classInfo（类信息），将这个类的信息注册到映射当中
    static Object* CreateObject(string name);   //工厂生产对象的接口
};

typedef Object* (*ObjectConstructorFn)(void);

class ClassInfo
{
public:
    /* data */
    string m_className;
    ObjectConstructorFn m_objectConstructor;
public:
    ClassInfo(const string className, ObjectConstructorFn ctor):m_className(className), m_objectConstructor(ctor)
    {
        Factory::Register(this);     //classInfo 的构造函数是传入类名和类对应的new函数然后自动注册进map 中
    }
    virtual ~ClassInfo() {}
    Object* CreateObject() const { return m_objectConstructor ? (*m_objectConstructor)() : 0; }
    bool isDynamic() const { return m_objectConstructor != nullptr; }
    ObjectConstructorFn getConstructor() const { return m_objectConstructor; }
};

#endif


