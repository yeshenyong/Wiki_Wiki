# C++ 单例设计

学名 `Meyers' Singleton` ，也就是 `Scott Meyers` 最早提出来的C++ 单例写法

```C++
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton inst;
        return inst;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 其他数据函数
    // ...

private:
    Singleton() { ... }
    // 其他数据成员
    // ...
};
```

注意这种单例写法需要C++11。因为是从C++11标准才开始规定 **static变量是线程安全的**。也就是说无需我们自己写加锁保护的代码，编译器能够帮我们做到



C++ 泛型单例

```c++
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton inst;
        return inst;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 其他数据函数
    // ...

private:
    Singleton() { ... }
    // 其他数据成员
    // ...
};
```























