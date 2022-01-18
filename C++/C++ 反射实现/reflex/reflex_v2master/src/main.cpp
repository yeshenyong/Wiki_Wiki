#include <iostream>
#include "../include/factory.h"
int main()
{
    Factory* fc = Factory::GetInstance();
    Object* obj = fc->CreateObject("B");
    // Object* objc = Factory::GetInstance()->CreateObject("C");
    if (obj) { delete obj; }
    // delete objc;
    return 0;
}

