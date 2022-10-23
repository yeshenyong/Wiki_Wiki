# AddressSanitizer

谷歌大法好

https://github.com/google/sanitizers

Google 内存错误检测工具

运行时间扩大两倍，valgrind 接近十倍，大型cpp 项目基本报废

使用步骤

- 用-fsanitize=address选项编译和链接你的程序。
- 用-fno-omit-frame-pointer编译，以得到更容易理解stack trace。
- 可选择-O1或者更高的优化级别编译

错误信息指出：

- 错误类型
- 错误操作发生线程
- 错误操作分配块位置

### heap use after free

```cpp
// RUN: clang -O -g -fsanitize=address %t && ./a.out
int main(int argc, char **argv) {
  int *array = new int[100];
  delete [] array;
  return array[argc];  // BOOM
}
```

```sh
=================================================================
==6254== ERROR: AddressSanitizer: heap-use-after-free on address 0x603e0001fc64 at pc 0x417f6a bp 0x7fff626b3250 sp 0x7fff626b3248
READ of size 4 at 0x603e0001fc64 thread T0
    #0 0x417f69 in main example_UseAfterFree.cc:5
    #1 0x7fae62b5076c (/lib/x86_64-linux-gnu/libc.so.6+0x2176c)
    #2 0x417e54 (a.out+0x417e54)
0x603e0001fc64 is located 4 bytes inside of 400-byte region [0x603e0001fc60,0x603e0001fdf0)
freed by thread T0 here:
    #0 0x40d4d2 in operator delete[](void*) /home/kcc/llvm/projects/compiler-rt/lib/asan/asan_new_delete.cc:61
    #1 0x417f2e in main example_UseAfterFree.cc:4
previously allocated by thread T0 here:
    #0 0x40d312 in operator new[](unsigned long) /home/kcc/llvm/projects/compiler-rt/lib/asan/asan_new_delete.cc:46
    #1 0x417f1e in main example_UseAfterFree.cc:3
Shadow bytes around the buggy address:
  0x1c07c0003f30: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003f40: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003f50: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003f60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003f70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
=>0x1c07c0003f80: fa fa fa fa fa fa fa fa fa fa fa fa[fd]fd fd fd
  0x1c07c0003f90: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd
  0x1c07c0003fa0: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd
  0x1c07c0003fb0: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fa fa
  0x1c07c0003fc0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003fd0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:     fa
  Heap righ redzone:     fb
  Freed Heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack partial redzone: f4
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  ASan internal:         fe
==6254== ABORTING
```

### Heap buffer overflow

```cpp
// RUN: clang -O -g -fsanitize=address %t && ./a.out
int main(int argc, char **argv) {
  int *array = new int[100];
  array[0] = 0;
  int res = array[argc + 100];  // BOOM
  delete [] array;
  return res;
}
```

```sh
=================================================================
==6226== ERROR: AddressSanitizer: heap-buffer-overflow on address 0x603e0001fdf4 at pc 0x417f8c bp 0x7fff64c0c010 sp 0x7fff64c0c008
READ of size 4 at 0x603e0001fdf4 thread T0
    #0 0x417f8b in main example_HeapOutOfBounds.cc:5
    #1 0x7fa97c09376c (/lib/x86_64-linux-gnu/libc.so.6+0x2176c)
    #2 0x417e54 (a.out+0x417e54)
0x603e0001fdf4 is located 4 bytes to the right of 400-byte region [0x603e0001fc60,0x603e0001fdf0)
allocated by thread T0 here:
    #0 0x40d312 in operator new[](unsigned long) /home/kcc/llvm/projects/compiler-rt/lib/asan/asan_new_delete.cc:46
    #1 0x417f1c in main example_HeapOutOfBounds.cc:3
Shadow bytes around the buggy address:
  0x1c07c0003f60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003f70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003f80: fa fa fa fa fa fa fa fa fa fa fa fa 00 00 00 00
  0x1c07c0003f90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1c07c0003fa0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x1c07c0003fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00[fa]fa
  0x1c07c0003fc0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003fd0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003fe0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0003ff0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x1c07c0004000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:     fa
  Heap righ redzone:     fb
  Freed Heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack partial redzone: f4
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  ASan internal:         fe
==6226== ABORTING
```

### Stack buffer overflow

```cpp
// RUN: clang -O -g -fsanitize=address %t && ./a.out
int main(int argc, char **argv) {
  int stack_array[100];
  stack_array[1] = 0;
  return stack_array[argc + 100];  // BOOM
}
```

```sh
=================================================================
==6240== ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7fff8098b2b4 at pc 0x417fe1 bp 0x7fff8098b0f0 sp 0x7fff8098b0e8
READ of size 4 at 0x7fff8098b2b4 thread T0
    #0 0x417fe0 in main example_StackOutOfBounds.cc:5
    #1 0x7fa3667c976c (/lib/x86_64-linux-gnu/libc.so.6+0x2176c)
    #2 0x417e54 (a.out+0x417e54)
Address 0x7fff8098b2b4 is located at offset 436 in frame <main> of T0's stack:
  This frame has 1 object(s):
    [32, 432) 'stack_array'
HINT: this may be a false positive if your program uses some custom stack unwind mechanism or swapcontext
      (longjmp and C++ exceptions *are* supported)
Shadow bytes around the buggy address:
  0x1ffff0131600: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131610: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131620: f1 f1 f1 f1 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131630: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131640: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x1ffff0131650: 00 00 00 00 00 00[f4]f4 f3 f3 f3 f3 00 00 00 00
  0x1ffff0131660: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131670: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131680: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff0131690: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1ffff01316a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:     fa
  Heap righ redzone:     fb
  Freed Heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack partial redzone: f4
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  ASan internal:         fe
==6240== ABORTING
```

### Global buffer overflow

```cpp
// RUN: clang -O -g -fsanitize=address %t && ./a.out
int global_array[100] = {-1};
int main(int argc, char **argv) {
  return global_array[argc + 100];  // BOOM
}
```

```sh
=================================================================
==6211== ERROR: AddressSanitizer: global-buffer-overflow on address 0x000000622314 at pc 0x417fee bp 0x7fff2e146300 sp 0x7fff2e1462f8
READ of size 4 at 0x000000622314 thread T0
    #0 0x417fed in main example_GlobalOutOfBounds.cc:4
    #1 0x7f1c10d2a76c (/lib/x86_64-linux-gnu/libc.so.6+0x2176c)
    #2 0x417ef4 (a.out+0x417ef4)
0x000000622314 is located 4 bytes to the right of global variable 'global_array (example_GlobalOutOfBounds.cc)' (0x622180) of size 400
Shadow bytes around the buggy address:
  0x1000000c4410: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c4420: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c4430: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c4440: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c4450: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x1000000c4460: 00 00[f9]f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
  0x1000000c4470: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c4480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c4490: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c44a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x1000000c44b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:     fa
  Heap righ redzone:     fb
  Freed Heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack partial redzone: f4
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  ASan internal:         fe
==6211== ABORTING
```

### Use after return

默认不检测 `stack-use-after-return` ，它会汇报给难以解释的 `stack-buffer` 溢出

如果想要测试，要加`ASAN_OPTIONS=detect_stack_use_after_return=1`

```cpp
// RUN: clang -O -g -fsanitize=address %t && ./a.out
// By default, AddressSanitizer does not try to detect
// stack-use-after-return bugs.
// It may still find such bugs occasionally
// and report them as a hard-to-explain stack-buffer-overflow.

// You need to run the test with ASAN_OPTIONS=detect_stack_use_after_return=1

int *ptr;
__attribute__((noinline))
void FunctionThatEscapesLocalObject() {
  int local[100];
  ptr = &local[0];
}

int main(int argc, char **argv) {
  FunctionThatEscapesLocalObject();
  return ptr[argc];
}
```

### Use after scope

可以在运行期禁用

```cpp
// RUN: clang -O -g -fsanitize=address -fsanitize-address-use-after-scope \
//    use-after-scope.cpp -o /tmp/use-after-scope
// RUN: /tmp/use-after-scope

// Check can be disabled in run-time:
// RUN: ASAN_OPTIONS=detect_stack_use_after_scope=0 /tmp/use-after-scope

volatile int *p = 0;

int main() {
  {
    int x = 0;
    p = &x;
  }
  *p = 5;
  return 0;
}
```

```sh
=================================================================
==58237==ERROR: AddressSanitizer: stack-use-after-scope on address 0x7ffc4d830880 at pc 0x0000005097ed bp 0x7ffc4d830850 sp 0x7ffc4d830848
WRITE of size 4 at 0x7ffc4d830880 thread T0
    #0 0x5097ec  (/tmp/use-after-scope+0x5097ec)
    #1 0x7ff85fa6bf44  (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
    #2 0x41a005  (/tmp/use-after-scope+0x41a005)

Address 0x7ffc4d830880 is located in stack of thread T0 at offset 32 in frame
    #0 0x5096ef  (/tmp/use-after-scope+0x5096ef)

  This frame has 1 object(s):
    [32, 36) 'x' <== Memory access at offset 32 is inside this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism or swapcontext
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-use-after-scope (/tmp/use-after-scope+0x5097ec) 
Shadow bytes around the buggy address:
  0x100009afe0c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe0d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe0e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe0f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe100: 00 00 00 00 00 00 00 00 00 00 00 00 f1 f1 f1 f1
=>0x100009afe110:[f8]f3 f3 f3 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x100009afe160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Heap right redzone:      fb
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack partial redzone:   f4
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==58237==ABORTING
```

### Initialization order bugs

惊喜：静态初始化顺序错误也能检测

静态初始化顺序错误是c++程序中与全局对象的构造顺序有关的一个常见问题。不同源文件中全局对象的构造函数的运行顺序未指定。Parashift c++ FAQ的10.14-10.18节很好地解释了问题是什么，以及如何纠正它。下面是一个简短的例子:

```cpp
$ cat tmp/init-order/example/a.cc
#include <stdio.h>
extern int extern_global;
int __attribute__((noinline)) read_extern_global() {
  return extern_global;
}
int x = read_extern_global() + 1;
int main() {
  printf("%d\n", x);
  return 0;
}

$ cat tmp/init-order/example/b.cc
int foo() { return 42; }
int extern_global = foo();
```

这里的值如果x取决于extern_global的值，它可能用“42”初始化，也可能不用“42”初始化，这取决于翻译单元的初始化器运行的顺序:

```sh
$ clang++ tmp/init-order/example/a.cc tmp/init-order/example/b.cc && ./a.out 
1
$ clang++ tmp/init-order/example/b.cc tmp/init-order/example/a.cc && ./a.out 
43
```

这样的错误很难被发现，而且可能一直被忽视，直到代码(或编译器或链接策略)中一些不相关的更改改变了代码行为，通常以意想不到的方式破坏代码。

#### AddressSanitizer中的初始化顺序检查器

AddressSanitizer在编译后的程序中插入检查，目的是检测初始化顺序问题。它们在默认情况下是关闭的，您必须传递运行时标志来启用它们。看到示例用法

#### Loose init-order checking

如果全局变量的初始化器从尚未初始化的另一个翻译单元动态地访问初始化的全局变量，则此模式将报告错误。使用`check_initialization_order=true`来激活它：

```sh
$ clang++ -fsanitize=address -g tmp/init-order/example/a.cc tmp/init-order/example/b.cc
$ ASAN_OPTIONS=check_initialization_order=true ./a.out
=================================================================
==26772==ERROR: AddressSanitizer: initialization-order-fiasco on address 0x000001068820 at pc 0x427e74 bp 0x7ffff8295010 sp 0x7ffff8295008
READ of size 4 at 0x000001068820 thread T0
    #0 0x427e73 in read_extern_global() tmp/init-order/example/a.cc:4
    #1 0x42806c in __cxx_global_var_init tmp/init-order/example/a.cc:7
    #2 0x4280d5 in global constructors keyed to a tmp/init-order/example/a.cc:10
    #3 0x42823c in __libc_csu_init (a.out+0x42823c)
    #4 0x7f9afdbdb6ff (/lib/x86_64-linux-gnu/libc.so.6+0x216ff)
    #5 0x427d64 (a.out+0x427d64)
0x000001068820 is located 0 bytes inside of global variable 'extern_global' from 'tmp/init-order/example/b.cc' (0x1068820) of size 4
SUMMARY: AddressSanitizer: initialization-order-fiasco tmp/init-order/example/a.cc:4 read_extern_global()
<...>
```

从某种意义上说，这种模式报告了存在的问题。如果初始化的顺序改变，它可能不会报告错误:

```sh
$ clang++ -fsanitize=address -g tmp/init-order/example/b.cc tmp/init-order/example/a.cc
$ ASAN_OPTIONS=check_initialization_order=true ./a.out
43
```

#### Strict init-order checking

如果全局变量的初始化器从另一个转换单元访问任何动态初始化的全局变量，则此模式将报告错误。使用附加变量strict_init_order=true来激活它:

```sh
$ clang++ -fsanitize=address -g tmp/init-order/example/b.cc tmp/init-order/example/a.cc
$ ASAN_OPTIONS=check_initialization_order=true:strict_init_order=true ./a.out
=================================================================
==27853==ERROR: AddressSanitizer: initialization-order-fiasco on address 0x0000010687e0 at pc 0x427f74 bp 0x7fff3d076ba0 sp 0x7fff3d076b98
READ of size 4 at 0x0000010687e0 thread T0
    #0 0x427f73 in read_extern_global() tmp/init-order/example/a.cc:4
<...>
```

通过这种方式，您还可能发现潜在的初始化顺序问题

#### False positives

- 当期望从另一个翻译单元访问已经初始化的全局时，或者当特定的构造顺序被强制执行时，例如使用共享库时，严格的初始化顺序检查可能会报错。（Strict init-order checking）
- 松散的初始化顺序检查可能会对动态初始化的全局变量报告假阳性，在初始化之前仍然可以安全地访问全局变量(例如，如果它们的构造函数不做任何操作)。（Loose init-order checking）

### Memory leaks

```cpp
$ cat memory-leak.c 
#include <stdlib.h>

void *p;

int main() {
  p = malloc(7);
  p = 0; // The memory is leaked here.
  return 0;
}
$ clang -fsanitize=address -g memory-leak.c
$ ./a.out 

=================================================================
==7829==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 7 byte(s) in 1 object(s) allocated from:
    #0 0x42c0c5 in __interceptor_malloc /usr/home/hacker/llvm/projects/compiler-rt/lib/asan/asan_malloc_linux.cc:74
    #1 0x43ef81 in main /usr/home/hacker/memory-leak.c:6
    #2 0x7fef044b876c in __libc_start_main /build/buildd/eglibc-2.15/csu/libc-start.c:226

SUMMARY: AddressSanitizer: 7 byte(s) leaked in 1 allocation(s).
```

### 底层原理

工具用法比较简单，这里想重点说说该工具的原理。

可参考文档：[https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm](https://link.zhihu.com/?target=https%3A//github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm)

由于是内存检测工具，其需要对每一次内存读写操作进行检查：
`*address = ...; // or: ... = *address;`

进行如下的逻辑判断：

```cpp
if (IsPoisoned(address)) {
  ReportError(address, kAccessSize, kIsWrite);
}
*address = ...;  // or: ... = *address;
```

如果指针读写异常，则统计及打印异常信息，可见整个工具的关键在于 `IsPoisoned` 如何实现，该函数需要快速而且准确。

内存映射

其将内存分为两块：
主内存：程序常规使用
影子内存：记录主内存是否可用等meta信息

如果有个函数 `MemToShadow` 可以根据主内存地址获取到对应的影子内存地址，那么内存检测的实现，可以改写为：

```cpp
shadow_address = MemToShadow(address);
if (ShadowIsPoisoned(shadow_address)) {
  ReportError(address, kAccessSize, kIsWrite);
}
```

影子内存

`AddressSanitizer` 用 1 byte 的影子内存，记录主内存中 8 bytes 的数据。

为什么是 8 bytes ，因为malloc分配内存是按照 8 bytes 对齐。

这样，8 bytes 的主内存，共构成 9 种不同情况：

- 8 bytes 的数据可读写，影子内存中的value值为 **0**
- 8 bytes 的数据不可读写，影子内存中的value值为 **负数**
- 前 k bytes 可读写，后 (8 - k) bytes 不可读写，影子内存中的value值为 **k** 。

如果 `malloc(13)` ，根据 8 bytes 字节对齐的原则，需要 2 bytes 的影子内存，第一个byte的值为 0，第二个byte的值为 5。

这时，整个判断流程，可改写为：

```cpp
byte *shadow_address = MemToShadow(address);
byte shadow_value = *shadow_address;
if (shadow_value) {
  if (SlowPathCheck(shadow_value, address, kAccessSize)) {
    ReportError(address, kAccessSize, kIsWrite);
  }
}

// Check the cases where we access first k bytes of the qword
// and these k bytes are unpoisoned.
bool SlowPathCheck(shadow_value, address, kAccessSize) {
  last_accessed_byte = (address & 7) + kAccessSize - 1;
  return (last_accessed_byte >= shadow_value);
}
```

主内存映射到影子内存

```
MemToShadow` 采用简单直接映射的方式
64-bit `Shadow = (Mem >> 3) + 0x7fff8000;`
32-bit `Shadow = (Mem >> 3) + 0x20000000;
```



例子

如何检测数组访问越界：

```cpp
void foo() {
  char a[8];
  ...
  return;
}
```

`AddressSanitizer` 将其改写为：

```cpp
void foo() {
  char redzone1[32];  // 32-byte aligned
  char a[8];          // 32-byte aligned
  char redzone2[24];
  char redzone3[32];  // 32-byte aligned
  int  *shadow_base = MemToShadow(redzone1);
  shadow_base[0] = 0xffffffff;  // poison redzone1
  shadow_base[1] = 0xffffff00;  // poison redzone2, unpoison 'a'
  shadow_base[2] = 0xffffffff;  // poison redzone3
  ...
  shadow_base[0] = shadow_base[1] = shadow_base[2] = 0; // unpoison all
  return;
}
```

如图：

![img](https://pic3.zhimg.com/80/v2-3857c140cd0ad2c3e6e8d61904d9ff3a_720w.webp)


将 `char a[8]` 两侧用 `redzone` 包夹，这样数组访问越界时，立马能够侦测。



参考

https://www.jianshu.com/p/3a2df9b7c353

https://github.com/google/sanitizers

