# CPU 与 GPU 到底有什么区别

转自：https://mp.weixin.qq.com/s/jPh5o5LXDWi7WogyN6AHvQ

今天简单聊聊CPU与GPU。

CPU 的故事我们聊得比较多了，之前也发布过很多关于 CPU 的文章，因此这里重点聊聊 GPU。

## 教授 vs 小学生

你可以简单的将CPU理解为学识渊博的教授，什么都精通。

而GPU则是一堆小学生，只会简单的算数运算。

可即使教授再神通广大，也不能一秒钟内计算出500次加减法。

**因此对简单重复的计算来说单单一个教授敌不过数量众多的小学生**，在进行简单的算数运算这件事上，500个小学生(并发)可以轻而易举打败教授。

![图片](https://mmbiz.qpic.cn/mmbiz_png/9BK2WL6kfcRXJEh4vVFVpISFwwaRAmv2mzJxibOzibhSWunYV7hNlic0b2GskKAstZaYiaPxWKXWYFjznlnxrc1cCA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

因此我们可以看到，CPU和GPU的最大不同在于架构。

CPU适用于广泛的应用场景(学识渊博)，可以执行任意程序。

而GPU则专为多任务而生，并发能力强，具体来讲就是多核，一般的CPU有2核、4核、8核等，而GPU则可能会有成百上千核：

![图片](https://mmbiz.qpic.cn/mmbiz_png/9BK2WL6kfcRXJEh4vVFVpISFwwaRAmv2aZgO3zGlNwbAJsBibczY5ZNtDlOZxPTL7BgOovx7aRfKJFBjL3DyF1w/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

可以看到，CPU内部cache以及控制部分占据了很大一部分片上面积，因此计算单元占比很少。

再来看看GPU，GPU只有很简单的控制单元，剩下的大部分都被计算单元占据，因此CPU的核数有限，而GPU则轻松堆出上千核：

![图片](https://mmbiz.qpic.cn/mmbiz_png/9BK2WL6kfcRXJEh4vVFVpISFwwaRAmv2IDAtOrzQaeHna4kOGQzdqKnZ1VGYur6zfXJ1t4wSQ3ZXg2LbkjtP1g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

只不过CPU中每个核的能力好比教授，而GPU的每个核的能力好比一个小学生。

你可能会想，为什么GPU需要这么奇怪的架构呢？

##### **为什么GPU需要这么多核心？**

想一想计算机上的一张图是怎么表示的？无非就是屏幕上的一个个像素：

![图片](https://mmbiz.qpic.cn/mmbiz_png/9BK2WL6kfcRXJEh4vVFVpISFwwaRAmv2eXEYw8XU6pM3YyeKTsaHeuTzHZBYTaWxhROOExOVl4ribKUN5HiaDw9w/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

我们需要为每个像素进行计算，**而且是相同的运算**。

就好比刚才例子中的小学生计算计加法一样。

注意，对于屏幕来说一般会有上百万个像素，如果我们要串行的为每一个像素进行运算效率就太低了。

因此我们可以让GPU中的每一个核心去为相应的像素进行计算，由于GPU中有很多核心，因此并行计算可以大幅提高速度。

现在你应该明白为什么GPU要这样工作了吧。

除了GPU的核心数比较多之外，GPU的工作方式也比较奇怪。

## 奇怪的工作方式

对CPU来说，不同的核心可以执行不同的机器指令。

coreA在运行word线程的同时coreB上可以运行浏览器线程，这就是所谓的多指令多数据，MIMD，(Multiple Instruction, Multiple Data)。

而GPU则不同，**GPU上的这些核心必须整齐划一的运行相同的机器指令**，只是可以操作不同的数据。

这就好比这些小学生在某个时刻必须都进行加法计算，不同的地方在于有的小学生可能需要计算1+1，有的要计算2+6等等，变化的地方仅在于操作数，这就是所谓的单指令多数据，SIMD，(Single Instruction, Multiple Data)。

因此我们可以看到GPU的工作方式和CPU是截然不同的。

除了这种工作方式之外，GPU的指令集还非常简单，不像CPU这种复杂的处理器。

如果你去看CPU的编程手册就会发现，CPU负责的事情非常多：中断处理、内存管理、IO等等，这些对于GPU来说都是不存在的。

可以看到GPU的定位非常简单，就是纯计算，GPU绝不是用来取代CPU的，CPU只是把一些GPU非常擅长的事情交给它，GPU仅仅是用来分担CPU工作的配角。

CPU和GPU是这样配合工作的：

![图片](https://mmbiz.qpic.cn/mmbiz_png/9BK2WL6kfcRXJEh4vVFVpISFwwaRAmv2HVc46Le0M82yAdzU48ug28xn5Wt700mAQZOUBtNw2icGpUrVEs8WzFg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

## GPU擅长什么

比较适合GPU的计算场景是这样的：**1)计算简单；2）重复计算**。

因此如果你的计算场景和这里的图像渲染相似那么使用GPU就很合理了。

因此对于图形图像计算、天气预报以及神经网络等都适用于GPU，哦对了，GPU还适合用来挖矿。

好啦，这个话题今天就到这里，希望这篇对大家理解GPU有所帮助