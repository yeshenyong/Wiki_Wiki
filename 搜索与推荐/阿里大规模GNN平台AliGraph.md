# 阿里大规模GNN平台AliGraph

转载自：https://zhuanlan.zhihu.com/p/111569270

AliGraph系统有以下几个部分组成：

- **Storage层**：Graph对象的逻辑存储，在其之下有一层抽象的文件接口，可适配多种数据源
- **Sample层**：提供采样的算子，且可独立扩展，不依赖系统框架
- **Operator层**：进行图语义操作的封装（算子层）

在系统之上是算法和业务应用。



### GNN 算法框架

![img](https://pic3.zhimg.com/80/v2-86c80d4953f18c22be4348553cb8db2a_1440w.jpg)

GNN 算法框架



### 存储层（Storage）

#### 图分区

图分区将图存储在不同的worker上。AliGraph内置4种图分区算法，可以根据不同的情况选择：

- **METIS**：处理稀疏图
- **顶点切割和边切割分区**：处理稠密图
- **2-D 分区**：woker 数量固定
- **流式分区策略**：边更新频繁的图



#### 单独存储属性

![img](https://pic2.zhimg.com/80/v2-7d626e3333e320053ed1ff84c98ebb9d_1440w.jpg)



- 顶点和其邻居使用邻接表存储关系；
- 通过构建两个索引分别对顶点和边上的属性进行存储，不存在邻接表中，这样可以节省存储空间减少特征存储的冗余。



#### **缓存重要顶点的邻居**

如果一个顶点 v 经常被其他顶点访问，可以将它的邻居存在顶点所在的work上，这样可以降低通信成本。然而，若顶点都去做这样的缓存，那么会产生很多邻居的副本。因此，通过线面的度量来评估每个顶点的重要性，满足重要性则对邻居进行缓存：

![img](https://pic1.zhimg.com/80/v2-d913a05203638dbb6f81f119fedab354_1440w.png)

顶点的重要性=k-hop入度/k-hop出度。由于顶点重要性是满足power-law分布的，一次只有很少量的顶点需要进行缓存。

整个分区和缓存的算法如下：

![img](https://pic2.zhimg.com/80/v2-7ee6ea22ba157e31fbb0f6bf9dbc862d_1440w.jpg)

- ASSIGN函数将根据u计算出 e 所在的woker 节点；
- 对于每个顶点 v，若重要性满足阈值要求，则在顶点所在work分区缓存顶点 v 的 1 到 k-hop 的邻居顶点。一般来讲，GNN算法中只需要缓存2-hop，阈值设置为为 0.2 左右。

![img](https://pic1.zhimg.com/80/v2-cc20c3c92255db58ecc37a562c2cc554_1440w.jpg)







### 采样层（Sample）

内置三种采样器：

- **遍历采样器**（TRAVERSE），用于从分区子图中，采样一批顶点或边。
- **领域采样器**（NEIGHBORHOOD）：生成顶点的上下文。该顶点的上下文可以是一个或多个 hop 邻居，用于对该顶点的编码。
- **负采样器**（NEGATIVE）：用于生成负样本以加速训练过程的收敛。(通常使用本地的worker，也可以全局)

这三类采样器在GNN中都是一些基本操作，TRAVERSE用于batch训练，NEIGHBORHOOD用于邻居节点的采样，而NEGATIVE用于NCE Loss的计算中。

![img](https://pic4.zhimg.com/80/v2-ccf9d979b5a06ecabb27b7f29037d957_1440w.jpg)

将图上的顶点进行分组。每个组都将与一个请求流桶相关，其中的操作（包括读取和更新）都与该组中的顶点有关。桶是一个无锁队列，每个 bucket 绑定一个 CPU Core，桶中的每个操作都按顺序处理。

![img](https://pic1.zhimg.com/80/v2-535b51ea41ebbbef8615c8f10da498dc_1440w.jpg)

### 算子层（Operator）

- **Aggregate算子**：对每个顶点的邻居信息进行聚合操作，生成邻居信息表示向量，可以是平均、max polling和LSTM
- **Combine算子**：将邻居信息表示向量和节点自身的向量进行合并操作，通常是加权求和。

将每个图卷积层产生的中间结果向量（节点embedding向量）缓存起来，所有节点和batch可以共享这一结果，在做下一层计算中从缓存中取上一层相应节点的向量做Aggragete再与自身向量做combine即可更新本层的embedding向量。

![img](https://pic1.zhimg.com/80/v2-a031bf11ef0f4022e5e8322e365ea074_1440w.jpg)

![img](https://pic3.zhimg.com/80/v2-f8f5bcd704e7e27810b191c8f881a4d6_1440w.jpg)



### **算法层**

在阿里内部开发了以下多个GNN算法：

**AHEP**

- 目的：减轻传播算法在异构网络上的计算和存储成本。
- 主要思想：在异构图中设定了一个度量，通过每个顶点的结构信息和特征来评估其重要性，根据设定的概率分布分别对不同类型的所有邻居进行采样。

**GATNE**

- 目的**：**异构节点和异构边的图的嵌入学习。
- 主要思想：每个节点在每种edge type下的embedding由base embedding和edge embedding组成，base embedding 是每个节点在每种edge type下共享的，而 edge embedding是通过相邻节点的edge embedding计算得到的。
- 模型：

![img](https://pic2.zhimg.com/80/v2-31132530f50962d30575509a8ceb0bf1_1440w.jpg)

- 伪代码：

![img](https://pic1.zhimg.com/80/v2-c3b586ec1de0463ed04dcb35ad5c4284_1440w.jpg)

**Mixture GNN**

- 目的：处理多模态的异构图。
- 主要思想：在损失函数上进行了改进，函数的下界可以用负采样近似。

![img](https://pic1.zhimg.com/80/v2-e4a1fc7770d038f4f7abc86a033c6d1c_1440w.png)

**Hierarchical GNN**

- 目的：通过组织层次结构学习图的层次表征。
- 主要思想：每个step中会将一些相似顶点聚类，更新邻接矩阵，得到cluster的向量表示，然后更新节点的特征表示。
- 模型：

![img](https://pic1.zhimg.com/80/v2-c49c08d64ca4302e72d63ffa4b5894ec_1440w.jpg)

**Evolving GCN**

- 目的：学习随时间变化的动态网络中的顶点嵌入。
- 主要思想：将link分为两种类型：正常演化和突发链接。在时间 t，在图 G(t) 上用GraphSAGE 生成图中每个顶点 的嵌入，再利用VAE和 RNN 模型对图 G(t+1) 上的正常和突发进行预测。

**Bayes GCN**

- 目的：集成了知识图谱嵌入和行为图嵌入。
- 主要思想：在嵌入任务中增加了一个任务修正项，具体实施时，对于每个实体 v，从一个0均值的高斯分布中采样出修正变量。























