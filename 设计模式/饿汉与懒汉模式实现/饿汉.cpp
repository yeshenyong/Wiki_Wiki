#include <iostream>
#include <stdlib.h>

using namespace std;
/* 饿汉模式 */ 
class singleton{
	private:
		singleton()
		{
			cout << "构造一个单例对象" << endl;
		}
		~singleton()
		{
			cout << "析构一个单例对象" << endl;
		}
		static singleton *instance;
	public:
		static singleton *getInstance();
};

/*
	类加载时初始化好 
*/
singleton* singleton::instance = new singleton();

singleton* singleton::getInstance()
{
	return instance;
}

int main()
{
	singleton *a1 = singleton::getInstance();
	singleton *a2 = singleton::getInstance();
	if(a1 == a2)cout << "the same class\n";
	
	return 0;
}
 
