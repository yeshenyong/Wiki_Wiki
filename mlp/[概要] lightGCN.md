# LightGCN

这篇论文是He Xiangnan 老师一作，发表在SIGIR 20上的一篇推荐系统的文章。

本文主要讨论的问题有三个：

1. 为什么叫lightGCN，究竟哪里light？
2. lightGCN是如何实现的？
3. performance有怎样的提升？

## 问题一：什么是lightGCN?

lightGCN 是相对于一个更加经典的图神经网络：Neural Graph Collaborative Filtering (NGCF)而言，省去了相邻节点间的内积部分，因而加快了运行速度，同时也保证了预测的精准度。lightGCN可以看作NGCF的简化版。

### **Neural Graph Collaborative Filtering (NGCF):**

NGCF的计算方式如下:

![img](https://pic3.zhimg.com/80/v2-b2a6319c1ce8207d5ddd248bb8e2bb46_720w.jpg)

如果当前节点是user ( ![[公式]](https://www.zhihu.com/equation?tex=e_u%5E%7B%280%29%7D) )，那么one-hop aggregation的方法就是取该节点的所有neighbour (item) embedding 并与当前节点做内积，再参照上面的计算方法计算出 ![[公式]](https://www.zhihu.com/equation?tex=e_u%5E%7B%281%29%7D) ，即包含当前user节点1-hop的subgraph的embedding。 以此类推，当计算到 ![[公式]](https://www.zhihu.com/equation?tex=e_u%5E%7B%28k%29%7D) 时即表示包含当前user节点k-hop的subgraph。

### **LightGCN化简了哪里？**

lightGCN在做上述的subgraph aggregation的时候拿掉了临近节点与当前节点的内积部分，直接用如下的normalized求和方法来进行1-hop aggregation。这里 ![[公式]](https://www.zhihu.com/equation?tex=%5Cmathcal%7BN%7D_u) 和 ![[公式]](https://www.zhihu.com/equation?tex=%5Cmathcal%7BN%7D_i) 分别代表以 ![[公式]](https://www.zhihu.com/equation?tex=u) 和 ![[公式]](https://www.zhihu.com/equation?tex=i) 为中心临近节点的数量。

![img](https://pic2.zhimg.com/80/v2-8bff67ae18e64b3f22a0a21ddf8908d9_720w.jpg)

采取这样的aggregation的方法，网络结构可以大幅化简。

## 问题二：lightGCN 是怎样实现的?

![img](https://pic2.zhimg.com/80/v2-c55e97f70743eee133893b5ec5a15d8d_720w.jpg)

lightGCN的网络结构如上图，分别用刚才提到的aggregation方法计算出1-hop到3-hop的subgraph embedding，再把他们一起通过下面的公式加权求和，得到最终的user和item embedding。

![img](https://pic3.zhimg.com/80/v2-5904c39a030ea4d1107fd4ff8442e73a_720w.jpg)

**Loss function:**

最终的训练方法是采用bpr-loss：

![img](https://pic3.zhimg.com/80/v2-0ff04caec8ce53750b84a91cebeb83ba_720w.png)

## 问题三：lightGCN 的performance有怎样的提升?

### **跟主流推荐方法的比较：**

![img](https://pic4.zhimg.com/80/v2-a2b60cb08a54ccf15fad816ec0961ee7_720w.jpg)

可以看到LightGCN相对主流的一些推荐系统方法来说效果确有提升，但这一部分篇幅较少，原文中也没有进行详细讨论，我们就不详细展开讲了。

### **Ablation study**

Ablation study作为对模型的详细分析部分，已经渐渐成为推荐系统文章中不可或缺的部分之一。在原文中ablation study的部分比较长，我们在这里捡重点来说

- Layer Combination 很重要： 这里主要指网络中需要把不同hop的subgraph embedding通过加权求和的方式组合在一起。同时相对于单层的GCN layer，这样的组合方式效果更好。



- Layer normalisation 也很重要：

![img](https://pic4.zhimg.com/80/v2-1a61a94cdbcf0357dbd0d56bea7c226f_720w.jpg)Layer normalisation的方法

这里指在计算1-hop aggregation时，上图的normalisation必不可少，可能会较大程度上影响最终的performance。



### 3.1.1 Light Graph Convolution

在LightGCN中，我们采用简单的加权和聚合器，而不使用特征变换和非线性激活。 LightGCN中的图卷积运算定义为：

![[公式]](https://www.zhihu.com/equation?tex=+e%5E%7B%28k%2B1%29%7D_u+%3D%5Csum_%7Bi+%5Cin+N_u%7D%5Cfrac%7B1%7D%7B%5Csqrt%7B%7CN_u%7C%7CN_i%7C%7D%7D+e%5E%7B%28k%29%7D_i%5C%5Ce%5E%7B%28k%2B1%29%7D_i+%3D%5Csum_%7Bu+%5Cin+N_i%7D%5Cfrac%7B1%7D%7B%5Csqrt%7B%7CN_u%7C%7CN_i%7C%7D%7D+e%5E%7B%28k%29%7D_u+)

### 3.1.2 层组合和模型预测

![[公式]](https://www.zhihu.com/equation?tex=e_u+%3D+%5Csum%5EK_%7Bk%3D0%7D+%CE%B1_ke%5E%7B%28k%29%7D_u+%3B+%5C%5C+ei+%3D+%5Csum%5EK_%7Bk%3D0%7D+%CE%B1_ke%5E%7B%28k%29%7D_i)

其中，αk≥0表示第k层嵌入在构成最终嵌入中的重要性。 可以将其视为要手动调整的超参数，也可以将其视为要自动优化的模型参数。 在我们的实验中，我们发现将αk统一设置为1 /（K +1）通常可获得良好的性能。 因此，我们无需设计特殊组件来优化αk，从而避免不必要地使LightGCN复杂化并保持其简单性。 我们执行图层组合以获得最终表示的原因有三点。 （1）随着层数的增加，嵌入会变得更加平滑[27]。 因此，仅使用最后一层是有问题的。 （2）不同层的嵌入具有不同的语义。 例如，第一层对具有交互作用的用户和项目强制执行平滑操作，第二层对在交互的项（用户）上重叠的用户（项目）进行平滑操作，更高的层则捕获较高级别的邻近度。 因此，将它们结合起来将使表示更加全面。 （3）将不同层的嵌入与加权和结合起来，可以捕获图卷积和自连接的效果，。

模型预测被定义为用户和项目最终表示的得分：

![[公式]](https://www.zhihu.com/equation?tex=%5Chat%7By%7D_%7Bui%7D+%3D+e%5ET+_u+e_i+)

### 3.1.3 矩阵形式

将用户与项目i交互矩阵R，其中$R_{ui}$ 为1表示u与i交互，反之为0。我们将领接矩阵表示为：

![[公式]](https://www.zhihu.com/equation?tex=A%3D%5Cbegin%7Bpmatrix%7D+0+%26+R%5C%5C+R%5ET+%26+0+%5Cend%7Bpmatrix%7D)

![[公式]](https://www.zhihu.com/equation?tex=E+%3D+%CE%B1_0E%280%29+%2B+%CE%B1_1E%281%29+%2B+%CE%B1_2E%282%29+%2B+...+%2B+%CE%B1_KE%28K%29%5C%5C+%3D+%CE%B1_0E%280%29+%2B+%CE%B1_1+%CB%9CAE%280%29+%2B+%CE%B1_2+%CB%9CA+2+E%280%29+%2B+...+%2B+%CE%B1_K+%CB%9CA+E%280%29)

其中 ![[公式]](https://www.zhihu.com/equation?tex=%5Cwidetilde%7BA%7D%3D+D%5E%7B-%5Cfrac%7B1%7D+%7B2%7D%7D+AD%5E%7B-%5Cfrac%7B1%7D+%7B2%7D%7D) 是对称归一化矩阵， ![[公式]](https://www.zhihu.com/equation?tex=%5Calpha) 是学习层组合系数。

### 3.2 模型训练

我们采用BPR作为损失函数训练模型：

![[公式]](https://www.zhihu.com/equation?tex=L_%7BBPR%7D+%3D+%E2%88%92+%5Csum%5EM_%7Bu%3D1%7D%5Csum_%7Bi%5Cin+N_u%7D%5Csum_%7Bj+%5Cnotin+N_u%7D+ln%CF%83%28+%5Chat%7By%7D_%7Bui%7D+%E2%88%92+%5Chat%7By%7D_%7Buj%7D+%29+%2B+%CE%BB%7C%7CE%280%29+%7C%7C2)

其中λ控制L2正则化强度。我们用Adam优化并且小批量方法。