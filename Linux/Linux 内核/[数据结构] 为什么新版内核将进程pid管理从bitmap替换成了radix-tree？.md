# 为什么新版内核将进程pid管理从bitmap替换成了radix-tree？

转自：[为什么新版内核将进程pid管理从bitmap替换成了radix-tree？ (qq.com)](https://mp.weixin.qq.com/s/0w7dJh0Gr4PokUBcy8rN7w)

## 一、旧的 bitmap 方式管理 pid

内核需要为每一个进程/线程都分配一个进程号。

如果每个使用过的进程号如果使用传统的 int 变量来存储的话会消耗很大的内存。假如内核要支持最大 65535 个进程，那存储这些进程号需要 65535*4 字节 = 262,140字节 ≈ 260 KB。

bitmap 可以极大地压缩整数的存储。如果使用 bitmap 来存储使用过的进程号，用一个 bit 表示对应的 pid 是否被使用过了。最大支持 65535 个进程的话，只需要 65535 / 8 = 8 KB 的内存就够用了。相比上面的 260 KB，内存节约的非常的多。

占用内存小还有一个特别大的优势，那就是遍历的时候，由于局部性特别好，CPU 缓存命中率特别的高，遍历的时候性能就会特别好。所以，之前很长的一段时间里，内核都是使用 bitmap 来管理所有的进程 pid。

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/BBjAFF4hcwpTfuj0XsfNycC3bz0SrKnhO4z9C2aQlOWChewuA9Zv0IibsmVlvicVW4W4ldbRFt4icTjiaZv7ic1wM1A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



- 100: 0,1,36
- 1000: 0,15,40
- 10000: 2,28,16
- 50000: 12,13,16
- 60000: 14,41,32

那么 100、1000、10000、50000、60000 这几个数组成的基数树的结构如下图所示

![图片](https://mmbiz.qpic.cn/sz_mmbiz_png/BBjAFF4hcwpTfuj0XsfNycC3bz0SrKnh59UfibicicrFicAY6kfsy1s29dD0tHBprrmarUEaok76UarO5QY4o4NSyQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## 三、基数树的性能效果

原理我们讲完了，我们再来看下使用基数树替代 bitmap 后的性能表现如何。这里我们直接引用该 patch 的提交者 Gargi Sharma 提供的实验数据。来自 https://lwn.net/Articles/735675/。

Gargi Sharma 在 10000 个进程的情况下分别统计了 ps、pstree 的耗时情况。

```shell
ps:
 With IDR API With bitmap
real 0m1.479s 0m2.319s
user 0m0.070s 0m0.060s
sys 0m0.289s 0m0.516s
pstree:
 With IDR API With bitmap
real 0m1.024s 0m1.794s
user    0m0.348s 0m0.612s
sys 0m0.184s 0m0.264s
```

可见，在使用了基数树后，ps 和 pstree 命令的耗时都缩短了不少，性能大约提升了有 50%