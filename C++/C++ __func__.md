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

