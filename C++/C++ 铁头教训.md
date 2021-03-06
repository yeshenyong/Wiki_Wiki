### 教训一：

时间：2022/3/22

记录时间：1:49（凌晨）

耗时：3h

内容：适用自动推导`decltype(auto)`作为返回值声明

错误原因：对模板元编程不熟悉，在实例化模板时，没搞清在同一编译单元中的定义顺序。不同`cpp` 文件编译单元不一致

1. 在调用前定义函数
2. 或者显示声明返回类型而不适用自动推导

​	这是不允许的，因为返回类型是根据函数定义中的返回语句推导出来的。如果没有函数定义，那么编译器就无法确定返回类型。

详细介绍：

https://blog.csdn.net/weixin_44974875/article/details/123650726



### 教训二：

时间：2022/7/2

记录时间：13:00

耗时：30min

主旨：C 和 C++ 不是一门语言

错误原因：

1. 在 C++ 中，`malloc` 请求得到的空间需要类型转换才能成功使用，不支持void * 直接给，需要强转
2. C++ 中，命名空间使用的struct 需要声明作用域才能看到struct，比如`UndirectedGraph::neighbors`  这样才能看到结构体



### 教训三：

时间：2022/7/2

记录时间：13:00

耗时：一下午

主旨：在类中声明一个不定长的数组的时候，最好不要用原生数组，而采用vector 替代

数组声明的时候没有初始化，也没有给大小，直接取元素应该不行吧

![59e6f7afaa1eb7a57af5d18408d8989.png](https://s3.bmp.ovh/imgs/2022/07/02/076ddc261289a4ad.png)

![a24d281f03ab20768ea2a1eea2e00f3.png](https://s3.bmp.ovh/imgs/2022/07/02/dd30f84a0fd26990.png)

原C 版代码就work





### 教训四：

时间：2022/7/7

记录时间：11:45

耗时：20分钟

主旨：在类中引用一个类，则强转不可以转成该类中成员

![54a3e28187f5942e.png](https://i.bmp.ovh/imgs/2022/07/07/54a3e28187f5942e.png)

错误版本

```cpp
#ifndef COROUTINE_CONTEXT_H
#define COROUTINE_CONTEXT_H
#include <cstddef>
#include <cstring>
#include <iterator>
namespace stack_co {

class Context final {
public:
	class Coroutine;
    using Callback = void (*)(Coroutine *);
};
} // namespace stack_co

#endif //COROUTINE_CONTEXT_H
```

正确版本

```cpp
#ifndef COROUTINE_CONTEXT_H
#define COROUTINE_CONTEXT_H
#include <cstddef>
#include <cstring>
#include <iterator>
namespace stack_co {
class Coroutine;	// diff
class Context final {
public:
    using Callback = void (*)(Coroutine *);
};
} // namespace stack_co

#endif //COROUTINE_CONTEXT_H
```

