#include "../include/Reflex.h"
#include "../include/ReflexBase.h"
#include <unordered_map>
#include <mutex>
using std::unordered_map;
using std::string;

static unordered_map< string, ClassInfo* > *classInfoMap = NULL;
static std::mutex mtx_;

Factory* Factory::GetInstance() 
{
    static Factory factory;
    return &factory;
}

bool Factory::Register(ClassInfo* ci)
{
    std::unique_lock<std::mutex> _(mtx_);
    if (!classInfoMap) {
        classInfoMap = new std::unordered_map< string, ClassInfo* >();
    }
    if (ci) {
        if (classInfoMap->find(ci->m_className) == classInfoMap->end()) {
            classInfoMap->insert(unordered_map< string, ClassInfo* >::value_type(ci->m_className, ci));
        }
    }
    return true;
}

Object* Factory::CreateObject(string name)
{
    std::unique_lock<std::mutex> _(mtx_);
    unordered_map< string, ClassInfo* >::const_iterator iter = classInfoMap->find(name);
    if (classInfoMap->end() != iter) {
        return iter->second->CreateObject();
    }
    return nullptr;
}


