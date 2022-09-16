# Considering Performance（考虑性能）

## Build Time

### Forward Declare When Possible

This:

```cpp
// some header file
class MyClass;

void doSomething(const MyClass &);
```

而不是:

```cpp
// some header file
#include "MyClass.hpp"

void doSomething(const MyClass &);
```

这也适用于模板:

```cpp
template<typename T> class MyTemplatedType;
```

这是一种减少编译时间和重建依赖关系的主动方法

注意:向前声明确实会阻止更多的内联和优化。建议对发布版本使用**链接时间优化或链接时间代码生成**

### Avoid Unnecessary Template Instantiations（避免不必要的模板实例化）

模板不能自由实例化。实例化许多模板，或者使用比所需代码更多的模板会增加编译后的代码大小和构建时间

更多示例请参见 [this article](http://blog2.emptycrate.com/content/template-code-bloat-revisited-smaller-makeshared).



### Avoid Recursive Template Instantiations（避免递归模板实例化）







