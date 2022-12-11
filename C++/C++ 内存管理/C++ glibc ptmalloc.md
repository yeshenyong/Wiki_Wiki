# C++ glibc ptmalloc

转自：https://mp.weixin.qq.com/s/pdv5MMUQ9ACpeCpyGnxb1Q

### 内存分配

glibc运行时库分配动态内存，底层用的是malloc来实现(new 最终也是调用malloc)，下面是malloc函数调用流程图：

![图片](https://mmbiz.qpic.cn/mmbiz_png/p3sYCQXkuHhRXIkKJ7KWooq2f0YibyXX9pxMalYAla7bvpXq014GOgeHNjhTibeXGMx0EIH3kSCNZDTjDoKvrx8w/640?wx_fmt=gif&wxfrom=5&wx_lazy=1&wx_co=1)



### 内存释放

![图片](https://mmbiz.qpic.cn/mmbiz_png/p3sYCQXkuHhRXIkKJ7KWooq2f0YibyXX9gXibFHj4CPV7HhBBK10XIsHlMUDibgFGpeZgkV7SZ6xEgQguaC2g55ZA/640?wx_fmt=gif&wxfrom=5&wx_lazy=1&wx_co=1)







通过前面对glibc运行时库的分析，基本就能定位出原因，是因为我们调用了free进行释放，但仅仅是将内存返还给了glibc库，而glibc库却没有将内存归还操作系统，最终导致系统内存耗尽，程序因为 OOM 被系统杀掉