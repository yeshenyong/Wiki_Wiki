# C++ 优雅访问private 变量

严格说，C++的禁止外部类访问private 是指在编译期，**运行期都是在内存中的数据，是可以做任何修改的。本文的方式讲的就是针对运行期如何修改**

```cpp
#include <iostream>

using namespace std;

class Base {
public:
	Base() {
		val = 1;
		val2 = 10;
		val3 = 100;
	}
	void print() {
		printf("val = %d, val2 = %d, val3 = %d\n", 
			val, val2, val3);
	}
public:
	int val;
private:
	int val2;
	int val3;
};

int main() {
	Base a;
	
	int *val = (int *)&a + 0;
	int *val2 = (int *)&a + 1;
	int *val3 = (int *)&a + 2;

	*val = 9;
	*val2 = 99;
	*val3 = 999;

	a.print();
	
	
	
	return 0;
}
```

