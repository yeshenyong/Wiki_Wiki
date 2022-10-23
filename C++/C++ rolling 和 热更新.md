# C++ rolling 和 热更新

https://howardlau.me/programming/c-cpp-hot-reload.html

如果软件是以敏捷开发模式做出的，几乎不可避免的要频繁升级程序，那么可以想象这会让用户多么烦恼

有时候在服务器程序中，需要不停止运行程序但是又要更新一些程序代码，这时候可以借助动态加载库来实现功能的热更新，不过不是在编译的时候链接动态链接库，而是在程序中手动控制加载和卸载

如果软件是以敏捷开发模式做出的，几乎不可避免的要频繁升级程序，那么可以想象这会让用户多么烦恼

动态链接库的手动使用很简单，主要有四个 API：`dlopen()` 用于打开 `.so` 文件，`dlsym()` 用于加载符号，`dlclose()` 用来卸载链接库以及 `dlerror()` 用来输出错误信息

通过输入函数名，获取动态链接库中的符号，然后调用函数：

```cpp
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;

typedef int (*cal_func)(int, int);
const char* LIB_PATH = "./libreco.so";

int main() {
    ino_t so_ino = -1;
    void* handle = nullptr;
    char* error = nullptr;

    for(;;) {
        string function_name;
        cout << "What function (type `quit` to exit): ";
        cin >> function_name;
        if (function_name == "quit") break;
        struct stat attr;
        if (stat(LIB_PATH, &attr) == 0 && attr.st_ino != so_ino) {
            if (handle) {
                dlclose(handle);
            }
            handle = dlopen(LIB_PATH, RTLD_LAZY);
            if (!handle) {
                cerr << dlerror() << endl;
                exit(EXIT_FAILURE);
            }

            cal_func func = reinterpret_cast<cal_func>(
                dlsym(handle, function_name.c_str()));
            if (error = dlerror()) {
                cerr << error << endl;
                continue;
            } else {
                int a, b;
                cout << "Input two numbers: ";
                cin >> a >> b;
                cout << func(a, b) << endl;
            }
        }
    }

    return 0;
}
// g++ -rdynamic -o main main.cc -ldl
```

```cpp
// AddSum.cc
#include <iostream>
extern "C" {
    int add(int a, int b) { return a + b; }
}
// 这里使用 extern "C" 关键字来导出符号，避免 C++ 的符号重整机制改变函数的名字。
// g++ -fPIC -shared AddSum.cc -o libreco.so
// 其中 -fPIC 选项代表生成位置无关代码，这是动态链接技术实现的关键，
// 然后 -shared 表示生成动态链接符号，这样其他程序可以动态加载符号并调用其中的函数
```

这里使用了 `stat()` 函数来判断动态库是不是已经被更新过，还有一个需要注意的地方是 `dlclose()` 一定要在 `dlopen()` 之前调用，否则得到的是缓存版本的动态库。`RTLD_LAZY` 表示的是懒加载模式，直到请求一个符号之前都不会加载符号相关的信息，其中的技术是 **GOT (Global Offset Table)** 和 **PLT (Procedure Linkage Table)**，具体内容可以在 CSAPP 一书的第七章找到相关介绍



![img](https://img1.imgtp.com/2022/10/17/bGwCesKz.png)

总结一下，就是用 `dlopen()`、`dlsym()`、`dlclose()` 等 API 实现模块的重新加载和符号的解析。具体的重加载时机和符号解析方式可以由我们自行决定。另外需要注意的一点是禁止在动态链接库中声明全局变量或者静态变量，因为每次重新加载的时候都会改变其内容。动态链接库中应当实现的是纯函数，只依赖于输入状态计算出输出状态











