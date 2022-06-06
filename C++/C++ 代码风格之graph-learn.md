1. C++ 头文件重复define：可以包括文件路径（如：GRAPHLEARN_COMMON_BASE_UNCOPYABLE_H_）
2. 对类的直接不可拷贝实现（拷贝构造函数、=运算符重载等这些的delete，不需要每个类都实现一遍）可以：

```C++
class Uncopyable {
public:
  Uncopyable(const Uncopyable& right) = delete;
  Uncopyable(Uncopyable&& right) = delete;
  Uncopyable& operator=(const Uncopyable& right) = delete;
  Uncopyable& operator=(Uncopyable&& right) = delete;

protected:
  Uncopyable() = default;
  ~Uncopyable() = default;
};
class ThreadPoolBase : private Uncopyable // 继承机制
```

