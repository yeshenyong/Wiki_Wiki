[图解｜低精度定时器原理 (qq.com)](https://mp.weixin.qq.com/s/UIVSw00D-agXVr2MMGrW6w)

本文主要介绍低精度定时器的实现，低精度定时器是一种比较廉价（占用资源较低）的定时器，如果对定时器的到期时间精度不太高的情况下，可以优先使用低精度定时

在 Linux 内核中，有两种类型的定时器：`高精度定时器` 与 `低精度定时器`。低精度定时器基于硬件的时钟中断实现的，其定时周期的粒度为 1/HZ ms。例如，内核 HZ 为 1000（也就是说 1 秒能够产生 1000 次时钟中断），那么低精度定时器的最小定时间隔为1ms；而高精度定时器可以实现纳秒级别的定时（实际的定时周期粒度与 CPU 的主频有关）

![图片](https://mmbiz.qpic.cn/mmbiz_png/ciab8jTiab9J6g94ua1TYtoqbt6ofWja3Xtj9tQ58RK1s7NCbotfeTNnB638ibHNIaI97icQ1IQbwpbyCyl2gFuPfQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

