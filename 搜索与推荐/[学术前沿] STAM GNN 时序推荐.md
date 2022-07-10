# STAM:A Spatiotemporal Aggregation Method for Graph Neural Network-based Recommendatio

转自：https://blog.csdn.net/CRW__DREAM/article/details/123553487

# 一、引言

基于GNN的推荐系统通常基于空间结构信息对近邻embedding进行聚合，从而忽略了时间序列信息的聚合，本文提出STAM方法根据时间和空间信息对近邻的一阶邻居节点embedding进行聚合，并且此方法可以和其他GNN方法结合，即只修改GNN方法中的聚合函数为STAM即可。

**本文的创新点：**

1. 强调了时间序列信息在基于GNN的邻居聚集推荐算法中的重要意义，说明聚类方法考虑结构和时间序列是有意义的。
2. 提出的STAM聚合方法可以很自然的替代现有的基于GNN方法进行推荐的算法中。



# 二、GNN方法导读

详情请见博客：[《基于推荐的GNN方法导读》](https://blog.csdn.net/CRW__DREAM/article/details/123560193?spm=1001.2014.3001.5501)

但是GNN有一个局限性：
他只考虑了网络中的连边的关系，而没有考虑过网络连边的时序关系（即user先预览了什么商品，后预览了什么商品）


![img](https://pic2.zhimg.com/80/v2-26724af4c437674e37ecf81ecfa8b295_1440w.jpg)

### **要点一：如何设计结合时间信息的聚合方式？**

GNN-based模型大家应当很熟悉了，关键步骤包括：①Embedding Layer、②Embedding Aggregation Layer、 ③Embedding Propagation Layer与④Prediction Layer。

今天的主角STAM正处于第三步”Embedding聚合部分“。目前的GNN的聚合方式有多种，不过大致可以分为四类：

1. ”**mean pooling**“，即以平等的方式对待所有邻居；
2. “**degree normalization**”，根据图结构为节点指定权重；
3. ”**attentive pooling**“，利用注意机制区分邻居的重要性；
4. ”**central node augmentation**“，考虑节点之间的相关性，并使用中心节点过滤邻居信息。

很遗憾上述方式都只涉及了图的空间结构信息，无法加入时间信息。![在这里插入图片描述](https://img-blog.csdnimg.cn/c8fedaa0716a43c19ae1349e28abe0a0.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBA5aCH56ak,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)



此图说明了两个人在不同时间预览了相同的商品，Sherry在t1 - t4时刻分别预览了ABCD四种商品，Amy在t1 - t4时刻预览了DCBA四种商品，要是按照普通GNN的方法的话，算法得到最后给两人推荐的结果是相同的，但若考虑了预览物品的先后顺序，即用STAM方法得到的两人被推荐商品并不一样。后者的推荐更为精准。



# 三、STAM方法

如图所示为STAM的总体流程，以时序序列Tu，Tv为输入，输出以一阶邻居生成的时空embedding。首先，根据时间关系，以用户u的一阶邻居节点（即交互的商品）构建用户u的时序序列Tu={v1, …, vs}，即交互的商品时间序列。同理构造商品对应的用户交互时间序列Tv={u1, …, us}，经过常用的embedding层，可以得到两个序列对应的embedding序列：

![在这里插入图片描述](https://img-blog.csdnimg.cn/d65bfd6fd9494905b05a91fb9afb21a7.png#pic_center)

上面的eut和evt可以通过[GNN的常规模型](https://blog.csdn.net/CRW__DREAM/article/details/123560193?spm=1001.2014.3001.5501)得到。
**下面是STAM最重要部分：**

对于每个节点对（u, v）从每个节点的一阶邻居学习到基于时间和空间上的embedding向量表示是很重要的。本文利用《Scaled Dot-Product Attention》文章中的方法引用Q, K, V发掘序列中的关系。采用《In International Conference on Machine Learning》文章的方法在序列embedding中加入位置embedding Pu={pv1, …, pvS}和Pv={pu1, …, puS}从而得到包含位置信息的序列表征：

![在这里插入图片描述](https://img-blog.csdnimg.cn/db65db28897343d2941462a4effd98b0.png)
将这些序列embedding组成矩阵可得：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0df18c09d71540be82c7779d66539ca5.png#pic_center)



然后通过自注意力得到新的embedding，以用户的序列为例，公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/080dbd225e1b4c45aa3791a4322b1681.png#pic_center)
Zu是一个时序输入向量矩阵，WQ/K/V是对于每一个user-item的矩阵对（u, v）共享的权重转换固定值。hTu是结合了时空序列特征的带有一阶邻居的输出向量矩阵函数
同理可以得到hTv：
![在这里插入图片描述](https://img-blog.csdnimg.cn/63f5f962aeee40c9b862ce18b1929165.png#pic_center)

为了提高STAM的模型能力和稳定性，结合多头注意力机制输出，公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/397ed95be077437ba5977c2a7cf63aa2.png#pic_center)



hu和hv是最后结合多头注意力机制得到的基于时间和空间的邻居向量的embedding表示，hiTu和hiTv代表通过Scaled Dot-Product Attention方法得到的第i层的向量矩阵的输出，FNN(·)是式子FNN(x)=xW0+b0。（W0和b0是可训练的参数）
同理，基于时空的邻居节点的所有向量表示也能用序列集合hu={hv1, …, hvs}和hv={hu1, …, hus}表示。
得到两个序列矩阵后，对其内的每个embedding求均值得到最终的序列embedding向量，公式如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/0f954b5ca7534a3c8a56ad4368191a59.png#pic_center)

### **要点二：基于LightGCN的应用实例**

STAM只是一种基于GNN的聚合方式，不改变基于 GNN 推荐的框架，可以很自然地插入到现有的基于 GNN 的推荐模型中。在model的后半部分，作者以Light-GCN为基础模型，介绍了如何在GNN模型中应用STAM，即要点1未涉及到的embedding传播和池化问题：

![img](https://pic4.zhimg.com/80/v2-e05b47f63009b9fda1a57b626b608153_1440w.png)

![img](https://pic2.zhimg.com/80/v2-666f5e00b44ff25f0b2f43ff5ea53525_1440w.jpg)

注意在embedding逐层传播公式里有个新鲜的变量：

![img](https://pic3.zhimg.com/80/v2-1c69d143334fd2785d701d7aa91799a6_1440w.jpg)

这是一个从 ![[公式]](https://www.zhihu.com/equation?tex=h_u) 和 ![[公式]](https://www.zhihu.com/equation?tex=h_v) 学习到的注意力权重矩阵，将其与邻接矩阵做内积，可以节约内存。



