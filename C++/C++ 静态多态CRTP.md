# C++ 静态多态CRTP



![img](https://pic2.zhimg.com/80/v2-867ba6950deac571f62759c24eacbcf9_720w.webp)



实现

```cpp
#include <iostream>

template<typename Args>
class Father {
public:
    Father() {}
    void func1() {
        printf("Father fun1\n");
        static_cast<Args*>(this)->func();
    }
    virtual ~Father() {}
};

class Child : public Father<Child> {
public:
    Child() {}
    void func() {
        printf("Child fun\n");
    }
};

int main() {
    Father<Child> *f = new Child();
    f->func1();
    return 0;
}
```



析构与多态删除

```cpp
// method 1
// 虚函数声明为析构函数

// method 2
template <typename D>
void destroy(B<D>* b) {
    delete static_cast<D*>(b);
}
```





参考

[Design Patterns With C++（八）CRTP（上） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/142407249)