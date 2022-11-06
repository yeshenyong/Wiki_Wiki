https://www.cnblogs.com/Braveliu/p/12217270.html

C99标准中的__func__预定义标识符功能，其基本功能就是返回所在函数的名字

在C++11中，标准甚至允许其使用在类或者结构体中

```cpp
#include <iostream>

using namespace std;

void MethodManager() {
	
	cout << __func__ << endl;
}

int main() {
	
	MethodManager();
	
	return 0;
}
```

 C/C++提供了三个宏__FUNCTION__，__FILE__和__LINE__来定位程序运行时的错误。程序预编译时预编译器将用所在的函数名，文件名和行号替换。当运行时错误产生后这三个宏分别能返回错误所在的函数，所在的文件名和所在的行号