https://www.cnblogs.com/magisk/p/8809922.html

boost：dynamic_bitset



### dynamic_bitset

 c++98标准为处理二进制数值提供了两个工具:vector<bool>和bitset.
**vector<bool>是对元素类型为bool的vector特化，内部并不真正存储bool值，而是以bit来压缩保存，使用代理技术来操作bit，造成的后果就是它很像容器，大多数情况下的行为与标准容器一致，但它不是容器，不满足容器的定义**。
 bitset与vector<bool>类似，同样存储二进制位，但它的大小固定，而且比vector<bool>支持更多的位运算。
 vector<bool>和biset各有优缺点：vector<bool>可以动态增长，但不能方便地进行位运算，bitset则正好相反，可以方便地容纳的二进制做位运算，但不能动态增长。
 boost.dynamic_bitset的出现恰好填补了这两种之间的空白，它类似标准库的bitset，提供丰富的位运算，同时长度又是动态可变的。
#include <boost/dynamic_bitset.hpp>
using namespace boost;

 boost.dynamic_bitset几乎与std::bitset相同，包括接口和行为，唯一区别是**boost.dynamic_bitset的大小是在构造函数中由参数指定的，而且运行时是动态可变的**

```cpp
dynamic_bitset<> db1;
dynamic_bitset<> db2(10);
dynamic_bitset<> db3(0x16, BOOST_BINARY(10101));//使用BOOST_BINARY宏，构造编译期二进制数，没有运行时开销，较db4使用string临时变量的构造效率高
dynamic_bitset<> db4(string("0100"));
dynamic_bitset<> db5(db3);
dynamic_bitset<> db6;
db6 = db4;

cout<<hex<<db5.to_ulong()<<endl;
cout<<db4[0]<<db4[1]<<<db4[2]<endl;//001
 dynamic_bitset内部按照由高到低的顺序存储二进制位
```

