https://cloud.tencent.com/developer/article/1136054

摘要

RBM 的用途和 Bitmap 很差不多（比如说索引），只是说从性能、空间利用率各方面更优秀了

简单列几个作为参考：

- Apache Lucene and derivative systems such as Solr and Elasticsearch,
- Metamarkets’ Druid,
- Apache Spark,
- Apache Hive,
- eBay’s Apache Kylin,
- ……





RBM 的主要思想并不复杂，简单来讲，有如下三条：

1. 我们将 32-bit 的范围 ([0, n)) 划分为 2^16 个桶，每一个桶有一个 Container 来存放一个数值的低16位；
2. 在存储和查询数值的时候，我们将一个数值 k 划分为高 16 位`(k % 2^16)`和低 16 位`(k mod 2^16)`，取高 16 位找到对应的桶，然后在低 16 位存放在相应的 Container 中；
3. [容器](https://cloud.tencent.com/product/tke?from=10680)的话， RBM 使用两种容器结构： Array Container 和 Bitmap Container。Array Container 存放稀疏的数据，Bitmap Container 存放稠密的数据。即，若一个 Container 里面的 Integer 数量小于 4096，就用 Short 类型的有序数组来存储值。若大于 4096，就用 Bitmap 来存储值



如下图，就是官网给出的一个例子，三个容器分别代表了三个数据集：

1. the list of the first 1000 multiples of 62
2. all integers [216, 216 + 100)
3. all even numbers in [2216, 3216)

![img](https://ask.qcloudimg.com/http-save/yehe-1051135/cfzmtfmj6l.png?imageView2/2/w/1620)





看完前面的还不知道在说什么？没关系，举个栗子说明就好了。现在我们要将 821697800 这个 32 bit 的整数插入 RBM 中，整个算法流程是这样的：

1. 821697800 对应的 16 进制数为 30FA1D08， 其中高 16 位为 30FA， 低16位为 1D08。
2. 我们先用二分查找从一级索引（即 Container Array）中找到数值为 30FA 的容器（如果该容器不存在，则新建一个），从图中我们可以看到，该容器是一个 Bitmap 容器。
3. 找到了相应的容器后，看一下低 16 位的数值 1D08，它相当于是 7432，因此在 Bitmap 中找到相应的位置，将其置为 1 即可。

![img](https://ask.qcloudimg.com/http-save/yehe-1051135/95a0qwry6i.png?imageView2/2/w/1620)

是不是很简单？然后换一个数值插入，比如说 191037，它的 16 进制的数值是 0002EA3D ，插入流程和前面的例子一样，不同的就在于， 高 16 位对应的容器是一个 Array Container，我们仍然用二分查找找到相应的位置再插入即可

RBM 的基本原理就这些，基于这种设计原理会有一些额外的操作要提一下。

请注意上文提到的一句话：

>  若一个 Container 里面的 Integer 数量小于 4096，就用 Short 类型的有序数组来存储值。若大于 4096，就用 Bitmap 来存储值。 

先解释一下为什么这里用的 4096 这个阈值？因为一个 Integer 的低 16 位是 2Byte，因此对应到 Arrary Container 中的话就是 2Byte * 4096 = 8KB；同样，对于 Bitmap Container 来讲，2^16 个 bit 也相当于是 8KB。

![img](https://ask.qcloudimg.com/http-save/yehe-5714147/vodr1m5pvr.png?imageView2/2/w/1620)

然后，基于前面提到的两种 Container，在两个 Container 之间的 Union (bitwise OR)  或者 Intersection (bitwise AND) 操作又会出现下面三种场景：

- Bitmap vs Bitmap
- Bitmap vs Array
- Array vs Array







本文介绍了一种新的位图压缩方案——Roaring。它将位图集项存储为32位整数，存储在一个空间效率高的两级索引中。与两种有竞争力的位图压缩方案相比，WAH和Concise，Roaring通常使用更少的内存和更快。当数据是有序的，位图需要存储长期连续的值(例如，在WIKILEAKS数据集)，替代方案，如Concise或WAH可能提供更好的压缩比。然而，即使在这些情况下，Roaring也可能更快。在未来的工作中，我们将考虑进一步提高性能和压缩比。我们可以添加新类型的容器。特别是，我们可以使用快速封装技术来优化数组容器[17]的存储使用。我们可以在算法中使用SIMD指令[18,19,20]。我们还应该回顾除交集和并集之外的其他操作，比如阈值查询[21]。我们计划进一步研究在信息检索方面的应用。我们有理由感到乐观:Apache Lucene(从5.0版开始)已经采用了一种Roaring格式[22]来压缩文档标识符