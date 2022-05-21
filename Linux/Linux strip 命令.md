# Linux 有一个命令你一定要知道

摘自：程序喵学习宝典

通过strip可以移除目标文件的符号信息，可以减少目标文件的体积，这里有几个问题：

1. 什么是符号？
2. 如何使用strip？
3. strip的作用是什么？
4. 动态链接库如果被strip后还能被链接成功吗？
5. 静态链接库如果被strip后还能被链接成功吗？

### 什么是符号？

**符号可以看作是链接中的粘合剂，整个链接过程需要基于符号才可以正确完成。链接过程的本质就是把多个不同的目标文件相 互粘到一起，像积木一样各有凹凸部分，但还是可以拼接成一个整体，这个将多个目标文件粘到一起的东西就是符号。可以将 函数和变量统称为符号，而函数名和变量名统称为符号名**

在 Linux 中可以通过一些命令来查看符号信息：

nm 命令：

```sh
 $nm test.o
 U _GLOBAL_OFFSET_TABLE_0000000000000000 T mainU puts
```

objdump 命令：

```sh
$objdump -t test.o

3. test.o: file format elf64-x86-64
4. 5. SYMBOL TABLE:
6. 0000000000000000 l df *ABS* 0000000000000000 test_c.cc
7. 0000000000000000 l d .text 0000000000000000 .text
8. 0000000000000000 l d .data 0000000000000000 .data
9. 0000000000000000 l d .bss 0000000000000000 .bss
10. 0000000000000000 l d .rodata 0000000000000000 .rodata
11. 0000000000000000 l d .note.GNU-stack 0000000000000000 .note.GNU-stack
12. 0000000000000000 l d .eh_frame 0000000000000000 .eh_frame
13. 0000000000000000 l d .comment 0000000000000000 .comment
14. 0000000000000000 g F .text 0000000000000017 main
15. 0000000000000000 *UND* 0000000000000000 _GLOBAL_OFFSET_TABLE_ 16. 0000000000000000 *UND* 0000000000000000 puts
```



readelf 命令：

```sh
$readelf -s test.o

3. Symbol table '.symtab' contains 12 entries:
4. Num: Value Size Type Bind Vis Ndx Name
5. 0: 0000000000000000 0 NOTYPE LOCAL DEFAULT UND
6. 1: 0000000000000000 0 FILE LOCAL DEFAULT ABS test_c.cc
7. 2: 0000000000000000 0 SECTION LOCAL DEFAULT 1
8. 3: 0000000000000000 0 SECTION LOCAL DEFAULT 3
9. 4: 0000000000000000 0 SECTION LOCAL DEFAULT 4
10. 5: 0000000000000000 0 SECTION LOCAL DEFAULT 5
11. 6: 0000000000000000 0 SECTION LOCAL DEFAULT 7
12. 7: 0000000000000000 0 SECTION LOCAL DEFAULT 8
13. 8: 0000000000000000 0 SECTION LOCAL DEFAULT 6
14. 9: 0000000000000000 23 FUNC GLOBAL DEFAULT 1 main
15. 10: 0000000000000000 0 NOTYPE GLOBAL DEFAULT UND _GLOBAL_OFFSET_TABLE_ 16. 11: 0000000000000000 0 NOTYPE GLOBAL 
```



### 如何使用 strip？

在 Linux 中可以使用 man strip 查看 strip 使用方法，最主要的就是移除所有符号的-s 参数，用于清除所有的符号信息：

```sh
strip -s xxx
```

在使用 strip 之前先使用 nm 查看下可执行程序的符号：

```sh
1. ~/test$ nm a.out
2. 0000000000200da0 d _DYNAMIC
3. 0000000000200fa0 d _GLOBAL_OFFSET_TABLE_ 4. 000000000000089b t _GLOBAL__sub_I__Z4funcPc
5. 0000000000000930 R _IO_stdin_used
6. w _ITM_deregisterTMCloneTable
7. w _ITM_registerTMCloneTable
8. 0000000000000852 t _Z41__static_initialization_and_destruction_0ii
9. 00000000000007fa T _Z4funcPc
10. 000000000000081c T _Z4funci
11. U _ZNSt8ios_base4InitC1Ev@@GLIBCXX_3.4
12. U _ZNSt8ios_base4InitD1Ev@@GLIBCXX_3.4
13. 0000000000201020 B _ZSt4cout@@GLIBCXX_3.4
14. 0000000000000934 r _ZStL19piecewise_construct
15. 0000000000201131 b _ZStL8__ioinit
16. U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@@GLIBCXX_3.4
17. 0000000000000b24 r __FRAME_END__ 18. 0000000000000940 r __GNU_EH_FRAME_HDR
19. 0000000000201010 D __TMC_END__ 20. 0000000000201010 B __bss_start
21. U __cxa_atexit@@GLIBC_2.2.5
22. w __cxa_finalize@@GLIBC_2.2.5
23. 0000000000201000 D __data_start
24. 00000000000007b0 t __do_global_dtors_aux 25. 0000000000200d98 t __do_global_dtors_aux_fini_array_entry
26. 0000000000201008 D __dso_handle
27. 0000000000200d88 t __frame_dummy_init_array_entry
28. w __gmon_start__ 29. 0000000000200d98 t __init_array_end
30. 0000000000200d88 t __init_array_start
31. 0000000000000920 T __libc_csu_fini
32. 00000000000008b0 T __libc_csu_init
33. U __libc_start_main@@GLIBC_2.2.5
34. 0000000000201010 D _edata
35. 0000000000201138 B _end
36. 0000000000000924 T _fini
37. 0000000000000688 T _init
38. 00000000000006f0 T _start
39. 0000000000201130 b completed.7698
40. 0000000000201000 W data_start
41. 0000000000000720 t deregister_tm_clones
42. 00000000000007f0 t frame_dummy
43. 000000000000083d T main
44. 0000000000000760 t 
```

当前这个可执行程序的文件大小是 8840 字节：

```sh
-rwxrwxrwx 1 a a 8840 Nov 29 14:54 a.out
```

使用 strip 清除符号信息：

```sh
~/test$ strip -s a.out
```

strip 后再查看可执行文件的符号信息：

```sh
~/test$ nm a.out
nm: a.out: no symbols
```

发现什么符号都没有了，但还是可以执行。 

strip 后的可执行程序文件大小是 6120 字节：

```sh
 -rwxrwxrwx 1 a a 6120 Nov 29 14:54 a.out
```

由此可见通过 strip 我们可以减少程序的体积。



### strip 的作用是什么？

前面已经大体介绍过，strip 最大的作用就是可以减少程序的体积，一般公司对发布的程序体积要求是极其严格的，strip 命令是 减少程序体积的一个很有效的方法。另一个作用就是提高了安全性，没有了这些符号，别人分析没有符号的程序会变得更加困 难。

### 动态链接库如果被 strip 后还能被链接成功吗？

先说答案，可以。 

先贴出两段代码：

```cpp
// shared.cc
#include <iostream>

void Print(int a) { std::cout << "Hello World " << a << std::endl; }
```

```cpp
// main.cc
#include <iostream>

void Print(int a);

int main() {
    Print(666);
    return 0;
}
```

将 shared.cc 编成一个动态链接库：

```sh
g++ shared.cc -o shared.so -shared -fPIC
```

使用readelf 查看链接库的符号信息：

```sh
root@iZwz9i5zftiq15tkzdk682Z:~/cpp_/unix/program cpp/strip# readelf -S shared.so
There are 28 section headers, starting at offset 0x1aa0:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.gnu.build-i NOTE             00000000000001c8  000001c8
       0000000000000024  0000000000000000   A       0     0     4
  [ 2] .gnu.hash         GNU_HASH         00000000000001f0  000001f0
       000000000000003c  0000000000000000   A       3     0     8
  [ 3] .dynsym           DYNSYM           0000000000000230  00000230
       00000000000001c8  0000000000000018   A       4     1     8
  [ 4] .dynstr           STRTAB           00000000000003f8  000003f8
       0000000000000189  0000000000000000   A       0     0     1
  [ 5] .gnu.version      VERSYM           0000000000000582  00000582
       0000000000000026  0000000000000002   A       3     0     2
  [ 6] .gnu.version_r    VERNEED          00000000000005a8  000005a8
       0000000000000040  0000000000000000   A       4     2     8
  [ 7] .rela.dyn         RELA             00000000000005e8  000005e8
       0000000000000108  0000000000000018   A       3     0     8
  [ 8] .rela.plt         RELA             00000000000006f0  000006f0
       0000000000000078  0000000000000018  AI       3    21     8
  [ 9] .init             PROGBITS         0000000000000768  00000768
       0000000000000017  0000000000000000  AX       0     0     4
  [10] .plt              PROGBITS         0000000000000780  00000780
       0000000000000060  0000000000000010  AX       0     0     16
  [11] .plt.got          PROGBITS         00000000000007e0  000007e0
       0000000000000008  0000000000000008  AX       0     0     8
  [12] .text             PROGBITS         00000000000007f0  000007f0
       0000000000000181  0000000000000000  AX       0     0     16
  [13] .fini             PROGBITS         0000000000000974  00000974
       0000000000000009  0000000000000000  AX       0     0     4
  [14] .rodata           PROGBITS         000000000000097d  0000097d
       000000000000000e  0000000000000000   A       0     0     1
  [15] .eh_frame_hdr     PROGBITS         000000000000098c  0000098c
       0000000000000034  0000000000000000   A       0     0     4
  [16] .eh_frame         PROGBITS         00000000000009c0  000009c0
       00000000000000bc  0000000000000000   A       0     0     8
  [17] .init_array       INIT_ARRAY       0000000000200de0  00000de0
       0000000000000010  0000000000000008  WA       0     0     8
  [18] .fini_array       FINI_ARRAY       0000000000200df0  00000df0
       0000000000000008  0000000000000008  WA       0     0     8
  [19] .dynamic          DYNAMIC          0000000000200df8  00000df8
       00000000000001d0  0000000000000010  WA       4     0     8
  [20] .got              PROGBITS         0000000000200fc8  00000fc8
       0000000000000038  0000000000000008  WA       0     0     8
  [21] .got.plt          PROGBITS         0000000000201000  00001000
       0000000000000040  0000000000000008  WA       0     0     8
  [22] .data             PROGBITS         0000000000201040  00001040
       0000000000000008  0000000000000000  WA       0     0     8
  [23] .bss              NOBITS           0000000000201048  00001048
       0000000000000008  0000000000000000  WA       0     0     1
  [24] .comment          PROGBITS         0000000000000000  00001048
       0000000000000029  0000000000000001  MS       0     0     1
  [25] .symtab           SYMTAB           0000000000000000  00001078
       0000000000000600  0000000000000018          26    46     8
  [26] .strtab           STRTAB           0000000000000000  00001678
       0000000000000330  0000000000000000           0     0     1
  [27] .shstrtab         STRTAB           0000000000000000  000019a8
       00000000000000f1  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
```

注意这里有 28 个符号段，主要有 symtab、strtab、dynsym、dynstr 段。 

strip 后再看下符号信息：

```sh
root@iZwz9i5zftiq15tkzdk682Z:~/cpp_/unix/program cpp/strip# strip -s shared.so
root@iZwz9i5zftiq15tkzdk682Z:~/cpp_/unix/program cpp/strip# readelf -S shared.so 
There are 26 section headers, starting at offset 0x1158:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.gnu.build-i NOTE             00000000000001c8  000001c8
       0000000000000024  0000000000000000   A       0     0     4
  [ 2] .gnu.hash         GNU_HASH         00000000000001f0  000001f0
       000000000000003c  0000000000000000   A       3     0     8
  [ 3] .dynsym           DYNSYM           0000000000000230  00000230
       00000000000001c8  0000000000000018   A       4     1     8
  [ 4] .dynstr           STRTAB           00000000000003f8  000003f8
       0000000000000189  0000000000000000   A       0     0     1
  [ 5] .gnu.version      VERSYM           0000000000000582  00000582
       0000000000000026  0000000000000002   A       3     0     2
  [ 6] .gnu.version_r    VERNEED          00000000000005a8  000005a8
       0000000000000040  0000000000000000   A       4     2     8
  [ 7] .rela.dyn         RELA             00000000000005e8  000005e8
       0000000000000108  0000000000000018   A       3     0     8
  [ 8] .rela.plt         RELA             00000000000006f0  000006f0
       0000000000000078  0000000000000018  AI       3    21     8
  [ 9] .init             PROGBITS         0000000000000768  00000768
       0000000000000017  0000000000000000  AX       0     0     4
  [10] .plt              PROGBITS         0000000000000780  00000780
       0000000000000060  0000000000000010  AX       0     0     16
  [11] .plt.got          PROGBITS         00000000000007e0  000007e0
       0000000000000008  0000000000000008  AX       0     0     8
  [12] .text             PROGBITS         00000000000007f0  000007f0
       0000000000000181  0000000000000000  AX       0     0     16
  [13] .fini             PROGBITS         0000000000000974  00000974
       0000000000000009  0000000000000000  AX       0     0     4
  [14] .rodata           PROGBITS         000000000000097d  0000097d
       000000000000000e  0000000000000000   A       0     0     1
  [15] .eh_frame_hdr     PROGBITS         000000000000098c  0000098c
       0000000000000034  0000000000000000   A       0     0     4
  [16] .eh_frame         PROGBITS         00000000000009c0  000009c0
       00000000000000bc  0000000000000000   A       0     0     8
  [17] .init_array       INIT_ARRAY       0000000000200de0  00000de0
       0000000000000010  0000000000000008  WA       0     0     8
  [18] .fini_array       FINI_ARRAY       0000000000200df0  00000df0
       0000000000000008  0000000000000008  WA       0     0     8
  [19] .dynamic          DYNAMIC          0000000000200df8  00000df8
       00000000000001d0  0000000000000010  WA       4     0     8
  [20] .got              PROGBITS         0000000000200fc8  00000fc8
       0000000000000038  0000000000000008  WA       0     0     8
  [21] .got.plt          PROGBITS         0000000000201000  00001000
       0000000000000040  0000000000000008  WA       0     0     8
  [22] .data             PROGBITS         0000000000201040  00001040
       0000000000000008  0000000000000000  WA       0     0     8
  [23] .bss              NOBITS           0000000000201048  00001048
       0000000000000008  0000000000000000  WA       0     0     1
  [24] .comment          PROGBITS         0000000000000000  00001048
       0000000000000029  0000000000000001  MS       0     0     1
  [25] .shstrtab         STRTAB           0000000000000000  00001071
       00000000000000e1  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
```

注意这里有 26 个符号段，主要有 dynsym、dynstr 段，这两个段 symtab、strtab 被清除掉。 而且依旧可以被链接成功并且成功执行程序：

而且依旧可以被链接成功并且成功执行程序：

```sh
~/test$ g++ main.cc -o main ./shared.so;./main
Hello World 666
```

为什么动态链接库被 strip 后还可以链接成功呢？因为 strip 只清除普通符号表，会保留动态符号表，即 dynsym、dynstr 段，而 动态链接依靠的就是动态符号表。

### 静态链接库如果被 strip 后还能被链接成功

也是先说答案，合理 strip 后就可以。 

先贴出两段代码：

```cpp
// static.cc
#include <iostream>

void Print(int a) { std::cout << "Hello World " << a << std::endl; }
```

```cpp
// main.cc
#include <iostream>

void Print(int a);

int main() {
    Print(666);
    return 0;
}
```

先将 static.cc 打包成 libsta.a：

```sh
gcc -c static.cc -o sta.o
ar -r libstat.a sta.o
```

查看下静态库的符号：

```sh
sta.o:
                 U _GLOBAL_OFFSET_TABLE_
000000000000008f t _GLOBAL__sub_I__Z5Printi
0000000000000046 t _Z41__static_initialization_and_destruction_0ii
0000000000000000 T _Z5Printi
                 U _ZNSolsEPFRSoS_E
                 U _ZNSolsEi
                 U _ZNSt8ios_base4InitC1Ev
                 U _ZNSt8ios_base4InitD1Ev
                 U _ZSt4cout
                 U _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
0000000000000000 r _ZStL19piecewise_construct
0000000000000000 b _ZStL8__ioinit
                 U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
                 U __cxa_atexit
                 U __dso_handle
```

将 libsta.a 库 strip 后发现什么符号都没有，且链接会失败：

```sh
strip -s libstat.a 
nm libstat.a

sta.o:
nm: sta.o: no symbols

$ g++ main.cc -o main -L. -lsta; ./main
./libsta.a: error adding symbols: Archive has no index; run ranlib to add one
collect2: error: ld returned 1 exit status
-bash: ./main: No such file or directory
```

那难道静态链接库就不能 strip 了吗？不 strip 的文件岂不是体积很大？

其实还是可以 strip 的，但需要合理的使用 strip，这里需要换一个 strip 的参数，就是--strip-unneeded，它确保 strip 掉的是没有 用的符号，保留用于链接的符号，尽管--strip-unneeded 不如-s 清除的彻底，但是保留了很多有用的信息，确保该链接库是可用 的

```sh
~/cpp_/unix/program cpp/strip# strip --strip-unneeded libstat.a 
~/cpp_/unix/program cpp/strip# nm libstat.a 

sta.o:
0000000000000000 T _Z5Printi
                 U _ZNSolsEPFRSoS_E
                 U _ZNSolsEi
                 U _ZNSt8ios_base4InitC1Ev
                 U _ZNSt8ios_base4InitD1Ev
                 U _ZSt4cout
                 U _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
                 U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
                 U __cxa_atexit
                 U __dso_handle
```

从上面可以看出：通过--strip-unneeded 即清除了部分符号的信息，还能保证库可用，减少程序体积。 

关于 strip，今天先介绍到这里，相信大家看完可以对 strip 理解的更深刻，并能更合理的使用 strip。关于编译和链接，大家可 以后台发送关键字“程序链接”了解更多细节。