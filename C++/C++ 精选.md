# C/C++







### value_type

`value_type`、`key_type`、`mapped_type`

```c++
map<string, int>::key_type v1; //string
map<string, int>::mapped_type v2; //int
map<string, int>::value_type v3; //pair<const string, int>
```

对于泛型编程来说，可能不知道具体类型是什么，所有就有了`key_type`、`mapped_type`、`value_type`这些类型。



### extern "C"

> ​	extern "C"的主要作用就是为了能够正确实现C++代码调用其他C语言代码。加上extern "C"后，会指示编译器这部分代码按C语言的进行编译，而不是C++的。由于C++支持函数重载，因此编译器编译函数的过程中会将函数的参数类型也加到编译后的代码中，而不仅仅是函数名；而C语言并不支持函数重载，因此编译C语言代码的函数时不会带上函数的参数类型，一般之包括函数名

extern “C”是由C++提供的一个连接交换指定符号，用于告诉C++这段代码是C函数。这是因为C++编译后库中函数名会变得很长，与C生产的不一致，造成C++不能直接调用C函数，加上extern “C”后，C++就能直接调用C函数了。(实现C与C++的混合编程)表示用到C语言中的变量或函数

**extern** 

extern 是变量或函数的申明，告诉编译器在其它文件中找这个变量或函数的定义。

这里需要的是“声明”，不是“定义”！根据C++标准的规定，一个变量声明必须同时满足两个条件，否则就是定义： 

- 声明必须使用extern关键字

- 不能给变量赋初值 

```c++
extern int a; //声明

int a; //定义

int a = 0; //定义

extern int a = 0; //定义
```



### 头文件查找

头文件一般有两种形式的写法：双引号和尖括号，如下：

```c++
#include <iostream>
#include "config.h"
```

**#include ""**

具体查找顺序为：

1. 当前目录（仅双引号形式）
2. 编译时指定的头文件目录（由`gcc -I` 参数指定）
3. 系统环境变量 `CPLUS_INCLUDE_PATH` （C++ 头文件）或 `C_INCLUDE_PATH` （C头文件）指定的目录
4. `gcc` 默认目录：`/usr/include;/usr/local/include;/usr/lib/gcc/x86_64-linux-gnu/5/include`(注：最后一个路径是gcc程序的库文件地址，各个用户的系统上可能不一样)

如果个目录下存在相同的文件，则先找到哪个使用哪个，这时顺序很重要。特别注意`<>` 不查找当前目录



**Tips**

> gcc 默认目录与安装gcc 时指定的-prefix有关，该值可以通过`gcc -v` 查看，具体目录可以通过`echo | g++ -v -x c++ -E -` 查看

了解头文件的查找规则，查找问题就得心应手

**注意**

除非必要，一般不建议将路径添加到头文件默认搜索路径。比较好的方式是写入`CMakeLists  `或者 `Makefile`  中（一次性生效，命令行输入即可，只对当前shell生效，重新登陆消失）

- C++：`export CPLUS_INCLUDE_PATH=xxx:$CPLUS_INCLUDE_PATH`
- C：`export C_INCLUDE_PATH=xxx:$C_INCLUDE_PATH`



若你非要这么干，添加路径到默认搜索路径

- 用户级生效：

修改用户的 `~/.bashrc` ，在文件最后添加上述指令。执行 `source ~/.bashrc` 或者重新登陆生效

- 全局级生效（慎重）：

修改 `/etc/profile` 文件，在文件最后添加上述命令，运行 `source /etc/profile` 后生效。对所有用户生效



### 库文件查找

上述头文件用于编译，库文件用于链接，编译通过了，还要链接通过。链接时库文件的查找顺序如下：

1. 编译时指定的库文件目录（由 `gcc -L` 指定）
2. 环境变量 `LIBRARY_PATH`  指定的目录
3. 系统默认目录：`/lib` 、`/usr/lib` 、 `/usr/local/lib`



一般用户安装的库会安装在 `/usr/local/lib`，系统自带的库位于 `/lib`、`/usr/lib` 中，用户自己编译的库可能就要使用 `-L` 参数制定了。

#### 动态库查找

​	编译通过了，还会有一个文件，就是运行动态库的使用。因为动态库是运行时加载的，所以还会有一个查找的顺序：

1. 编译时制定的动态库搜索路径（通过 `gcc` 的参数 `-WL,-rpath`  指定。当指定多个动态库搜索路径时，路径之间用冒号 `:` 分隔）
2. 环境变量 `LD_LIBRARY_PATH` 指定的动态库搜索路径（路径之间用冒号 `:` 分隔）
3. 配置文件 `/etc/ld.so.conf` 中指定的动态库搜索路径
4. 默认的动态库搜索路径 `/lib:/usr/lib`



**注意**

​	库文件的查找默认是不查找当前目录的，也就是说，即使使用的库位于同一个目录内，也需要指定才能使用。

​	除非必要，一般不建议添加路径到库文件默认搜索路径（和头文件一个道理）

比较好的方式时写入到 `CMakeLists` 或者 `Makefile` 中

与头文件默认路径的添加方法类似，添加的命令如下：

```sh
# 动态链接库搜索路径
export LD_LIBRARY_PATH=xxx:$LD_LIBRARY_PATH
# 静态链接库搜索路径
export LIBRARY_PATH=xxx:$LIBRARY_PATH
```

 



### 高性能C++ 库

https://github.com/fmtlib/fmt  





文章借鉴

- https://blog.csdn.net/guotianqing/article/details/104224439
- 
