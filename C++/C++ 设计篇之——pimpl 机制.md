# C++ 设计篇之——PImpl 机制

源码仓库：https://github.com/yeshenyong/practice_cpp/tree/master/pimpl

### 什么是PImpl 机制

Pointer to implementation（PImpl ），通过将类的实现细节放在一个单独的类中，从其对象表示中删除它们，通过一个不透明的指针访问它们（**cppreference 是这么说的**）

通过一个私有的成员指针，将指针所指向的类的内部实现数据进行隐藏

```c++
class Demo {
public:
	...
private:
	DemoImp* imp_;
}
```

### 为什么用PImpl  机制

个人拙见

1. C++ 不像Java 后端型代码，能有行业定式的列目录名形成规范（controller、Dao等）
2. 隐藏实现，降低耦合性和分离接口（隐藏类的具体实现）
3. 通过编译期的封装（隐藏实现类的细节）

业界实现

1. [优秀开源代码](https://github.com/alibaba/graph-learn/blob/6fa48588cfb44b4b1e7abb3b2ceda10e4d55c1c3/graphlearn/src/common/rpc/notification.cc#L32)有实现

### PImpl  实现

#### 方法一

`cook_cuisine.h`

```cpp
#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

//  Pointer to impl ementation

class CookImpl;

// 后厨
class Cook {

public:
    Cook(int, const std::vector<std::string>&);
    ~Cook();
    std::vector<std::string> getMenu();     /* 获取菜单 */
    uint32_t getChefNum();                  /* 获取厨师数量 */

private:
    CookImpl* impl_;
};
typedef std::shared_ptr<Cook> CookPtr;		// 美妙的typedef 懒人工具
```

`cook_cuisine.cc`

```cpp
#include "cook_cuisine.h"

class CookImpl {
public:
    CookImpl(uint32_t checf_num, const std::vector<std::string>& menu):checf_num_(checf_num), menu_(menu) {}
    std::vector<std::string> getMenu();
    uint32_t getChefNum();

private:
    uint32_t checf_num_;
    std::vector<std::string> menu_;
};

std::vector<std::string> CookImpl::getMenu() {
    return menu_;
}

uint32_t CookImpl::getChefNum() {
    return checf_num_;
}

Cook::Cook(int chef_num, const std::vector<std::string>& menu) {
    impl_ = new CookImpl(chef_num, menu);
}

Cook::~Cook() {
    delete impl_;
}

std::vector<std::string> Cook::getMenu() {
    return impl_->getMenu();
}

uint32_t Cook::getChefNum() {
    return impl_->getChefNum();
}
```

#### 方法二

`cook_cuisine.h`

```cpp
#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "cook_cuisine_imp.h"

// 后厨
class Cook {

public:
    Cook(int, const std::vector<std::string>&);
    ~Cook();
    std::vector<std::string> getMenu();     /* 获取菜单 */
    uint32_t getChefNum();                  /* 获取厨师数量 */

private:
    CookImplPtr impl_;
};
typedef std::shared_ptr<Cook> CookPtr;
```

`cook_cuisine.cc`

```cpp
#include "cook_cuisine.h"

Cook::Cook(int chef_num, const std::vector<std::string>& menu) {
    impl_.reset(new CookImpl(chef_num, menu));
}

Cook::~Cook() {
    
}

std::vector<std::string> Cook::getMenu() {
    return impl_->getMenu();
}

uint32_t Cook::getChefNum() {
    return impl_->getChefNum();
}
```

`cook_cuisine_imp.h`

```cpp
#pragma once

#include <vector>
#include <unordered_map>
#include <memory>


class CookImpl {
public:
    CookImpl(uint32_t checf_num, const std::vector<std::string>& menu):checf_num_(checf_num), menu_(menu) {}
    std::vector<std::string> getMenu();
    uint32_t getChefNum();

private:
    uint32_t checf_num_;
    std::vector<std::string> menu_;
};
typedef std::shared_ptr<CookImpl> CookImplPtr;
```

`cook_cusine_imp.cc`

```cpp
#include "cook_cuisine_imp.h"

std::vector<std::string> CookImpl::getMenu() {
    return menu_;
}

uint32_t CookImpl::getChefNum() {
    return checf_num_;
}
```

`main.cc`

```cpp
#include "cook_cuisine.h"
#include <iostream>


using namespace std;    // Testing, 平时开发可千万别用这句

int main() {
    int checf_num = 10;
    const std::vector<std::string> menus = { "Chicken", "Beef", "Noodle", "Milk" };
    CookPtr cook(new Cook(checf_num, menus));

    auto cook_menu = cook->getMenu();
    auto cook_checf_num = cook->getChefNum();

    cout << "======================Chinese Cook======================\n";

    cout << "============Checf: " << cook_checf_num << " people\n";

    cout << "==========Menu\n";

    for (size_t i = 0; i < cook_menu.size(); i++) {
        cout << "============" << i + 1 << " : " << cook_menu[i] << "\n";
    }

    return 0;
}
```

`CMakeLists.txt`

```sh
mkdir build
cd build
cmake ..
```

### PImpl 缺点

1. 空间开销：每个类都需要额外的指针内存指向实现类
2. 时间开销：每个类间接访问实现的时候多一个间接指针操作的开销
3. 阅读开销：使用、阅读和调试上带来一些不便（不是啥问题）

### 总结

每种设计方法都有它的优点和缺点

PImpl 用一些内存空间和额外类的实现换取耦合性的下降，是可以接受的



但重点在：**在性能/内存要求**不敏感处，PImpl 技术才更优不错的发挥舞台

极端例子：

> 你不可能在斐波那契的实现中还加个PImpl 机制，多此一举





















