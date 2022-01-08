# C++ Trick：什么时候需要前置声明？



经常有C++ 开发的小伙伴提问：

> C++ 中要使用类A时，什么时候#include "a.h，什么时候用class
>
>  A 声明呢？



通常来说，你都不需要主动去写`class A`这种前置声明。include能编译通过的时候都不要去写前置声明，应该仅当出现了头文件循环依赖导致编译失败的时候，才去考虑去写前置声明！

头文件循环依赖，就是说两个头文件互相include了对方，这样编译会出问题。举个例子。

有a.h(里面用了类型B的指针，所以include了b.h）：

```c++
#pragma once

#include "b.h"
class A {
public:
    A():_b(nullptr) {}
    ~A() {}

    void set_b(B* b) {
        _b = b;
    }
    B* get_b() {
        return _b;
    }

private:
    B* _b;
};
```

有b.h:

```c++
#pragma once
#include "a.h"

class B {
public:
    B(int i):_i(i) {}
    ~B() {}

    int i() {
        return _i;
    }

    void foo(A& a) {
        B* b = a.get_b();
        b->_i += _i;
    }

private:
    int _i;
};
```

有main.cpp （包含main函数）

```c++
#include "a.h"
#include "b.h"
#include <iostream>
using namespace std;

int main() {
    A a;
    B b(3);
    a.set_b(&b);

    B b2(7);
    b2.foo(a);

    cout << a.get_b()->i() << endl;
    return 0;
}
```

编译main.cpp失败，报错：

```c++
./b.h:13:14: error: unknown type name 'A'
 void foo(A& a) {
 ^
1 error generated.
```

修改方法，**因为a.h中只出现了类型B的指针，而未调用其成员函数或成员变量**，故可以修改a.h删除include "b.h"，增加类型B的前置声明。

```c++
#pragma once

class B; // 前置声明！
class A {
public:
    A():_b(nullptr) {}
    ~A() {}

    void set_b(B* b) {
        _b = b;
    }
    B* get_b() {
        return _b;
    }

private:
    B* _b;

};
```

编译main.cpp通过。

当然前置声明也不是万能的解药，请注意前面的加粗黑字：

> 因为a.h中只出现了类型B的指针，而未调用其成员函数或成员变量，故……

换言之，如果a.h中使用了类型B的成员函数，则无法通过更改为前置声明的方式，来让编译通过。

比如：

```c++
#pragma once
#include <iostream>
class B;
class A {
public:
    A():_b(nullptr) {}
    ~A() {}

    void set_b(B* b) {
        std::cout<< b->i() << std::endl; // !使用了B的成员函数
        _b = b;
    }
    B* get_b() {
        return _b;
    }

private:
    B* _b;

};
```

编译报错：

```c++
./a.h:10:22: error: member access into incomplete type 'B'
        std::cout<< b->i() << std::endl;
                     ^
./a.h:3:7: note: forward declaration of 'B'
class B;
```

**这时候只能老老实实地改代码，重新梳理并设计类A和类B的关系！**

### 总结

> 看起来有点乱，记不住？其实不难理解，因为对C++而言，不管是什么指针，它的大小都是确定的。所以只要a.h中只是出现B的指针（或引用）而没有调用其具体的成员函数，C++编译器是可以不去在此时理解B的具体定义的（故只添加class B的声明即可），一旦a.h中用到了B的成员函数，则不然。



摘自

https://mp.weixin.qq.com/s/7GYBh-u-84TKXBXB_GyEKA