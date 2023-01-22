# gcc 命令详解



`--print-file-name`

打印动态库绝对路径

```sh
gcc --print-file-name=linux-vdso.so.1
linux-vdso.so.1
```





`-print-search-dirs`

查看 gcc 在编译、链接过程中的共享库搜索路径

```sh
gcc --print-search-dirs
install: /usr/lib/gcc/x86_64-linux-gnu/11/
programs: =/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/bin/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/bin/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/bin/
libraries: =/usr/lib/gcc/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/lib/x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/lib/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu/11/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../lib/:/lib/x86_64-linux-gnu/11/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/11/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/lib/:/usr/lib/gcc/x86_64-linux-gnu/11/../../../:/lib/:/usr/lib/
```







