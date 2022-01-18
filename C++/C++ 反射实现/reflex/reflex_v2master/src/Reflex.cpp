#include "../include/factory.h"
#include "../include/ReflexBase.h"
#include <mutex>
#include <unordered_map>

using std::unordered_map;
using std::string;

static unordered_map< string, ClassInfo* > *classInfoMap;
static std::mutex mtx_;

bool Factory::Register(ClassInfo* ci)
{
    std::unique_lock<std::mutex> _(mtx_);
    if (!classInfoMap) {
        classInfoMap = new std::unordered_map< string, ClassInfo* >();
    }
    if (ci) {
        std::cout << "before insert\n" << ci->m_className << std::endl;
        if (classInfoMap->find(ci->m_className) == classInfoMap->end()) {
            std::cout << "in find\n" << std::endl;
            classInfoMap->insert(make_pair( ci->m_className, ci ));
            std::cout << ci->m_className << "Reigister\n";
        }
    }
    return true;
}

Object* Factory::CreateObject(string name)
{
    std::unique_lock<std::mutex> _(mtx_);
    auto iter = classInfoMap->find(name);
    if (classInfoMap->end() != iter) {
        return iter->second->CreateObject();
    }
    return nullptr;
}

Factory* Factory::GetInstance() {
    static Factory factory;
    return &factory;
}

Factory::~Factory() {
    if(classInfoMap) {
        std::cout << "release classInfoMap" << std::endl;
        delete classInfoMap;
    }
}


