#include <iostream>
#include "../include/Reflex.h"
int main()
{
    Object* obj = Factory::GetInstance()->CreateObject("B");
    Object* objc = Factory::GetInstance()->CreateObject("C");
    delete obj;
    delete objc;
    return 0;
}

