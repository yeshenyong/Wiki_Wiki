# 「工业实践」在内容推荐场，阿里是如何用GNN做召回的？

转载来源：https://mp.weixin.qq.com/s/RN_eO5CxdORmRnR4dRDR5A

> 每平每屋是阿里巴巴旗下家居家装平台，涵盖淘宝每平每屋家居频道、每平每屋设计家、每平每屋App、每平每屋制造业等家居全链路服务，为消费者提供了2D图文、3D样板间、3D虚拟直播、3D短视频、VR全屋漫游等丰富多元的家居内容，逐渐成为年轻一代生活灵感与家居装修的向导之一。

业务方向：淘系内的每平每屋



C端业务在淘内主要以每平每屋家居频道进行承接，提供场景化的导购服务。频道内每件单品都不再孤立呈现，而是以场景化内容搭配推荐为主。消费者只需在淘宝搜索“每平每屋”或点击首页猜你喜欢的“每平每屋”内容卡片即可进入频道，通过点击详情页挂载的商品可进行收藏、加购、购买等行为，获得沉浸式购物体验。

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJMIabYQU4ImUaWUrXKNibJ4ia4NRWfia8qJAYwmFMu6zVPLp5kE9iaq6EsA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



目前每平每屋频道内容推荐个性化召回主要分为**U-I-I**和**深度U-I**两大类。



1. **U-I-I召回：根据用户属性特征、引流内容或点击的内容，基于内容ID、内容所属的风格与空间、内容挂载的商品和类目，通过i2i、c2i等方式召回相关的内容，具有较高的精度。**
2. 深度U-I：根据用户的点击行为，将用户、内容转化为向量，通过向量检索技术召回topk，变“精确匹配”为“模糊查找”，具有较好的多样性和发散性。



因为淘宝首页猜你喜欢引流结构的特点，**部分用户对所见即所得以外模块浏览心智较低**。每日访问每平每屋频道的用户中，由淘宝首页频道卡片引流进入的零少行为用户占比较高，这使得我们在对用户兴趣建模时面临着严重的**用户行为稀疏**的问题。而用户的交互行为往往存在一定的流行度偏差，例如，用户点击某个内容受到收藏数、运营活动、展示位置靠前等诸多因素影响，而非个人兴趣偏好驱动，这就造成少数流行内容包含了大多数交互的**马太效应**，这一点在零少行为用户群体中更为明显。对这样分布特点的数据进行建模时，模型会“偷懒”地倾向于推荐流行内容去拟合，进而弱化了对用户真实意图的挖掘。因此，在优化频道内内容召回时，我们主要考虑了如下问问题，详细的实践介绍将在后面的章节展开。



1. **深度挖掘side info：**在用户与内容缺乏交互行为的情况下，基于GNN模型建模高阶关系，并融合side info，如用户点击的内容所挂载的商品、类目、风格、空间等，以建立起更多的关联。（挖掘更多的信息）
2. **对用户交互行为中的流行度偏差去偏**：基于GNN解耦模型挖掘用户泛群体的真实兴趣意图进行召回，以缓解零少行为用户推荐马太效应。



#### 挖掘side info

通过分析线上召回链路发现，**深度U-I模型召回的曝光pv、uv占比较低**，**而占比较高的则为i2i召回**。值得注意的是，**现有的i2i召回候选集在构建时仅仅考虑了用户点击行为，且主要是在做准。由于没有综合考虑内容侧丰富的side info，在召回结果的发散性上仍有较多的优化空间**。频道内每条内容挂载了多个商品锚点，点击商品锚点跳转到商品的详情页。此外，从内容详情页不难发现，除了挂载的多个商品外，内容还包含空间、风格和设计师等信息。概言之，内容关联了商品、内容、设计师等丰富且多维的side info，对于理解内容、优化召回链路提供了帮助。



##### 基于 single-view 的图表征学习

single-view（单视图）

在第一阶段，我们根据用户在每平每屋频道内行为数据构建content-content同构网络，并将内容挂载的商品、类目、风格、空间等side info作为节点的属性特征，融合进图的节点表征过程中。具体地，每一个节点对应一条内容![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJS7xUcAItLZo0ejT8YadvR5gSdEN4ZicVmI4GpINibpMvnr7jEYBwIXeA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)。用户在某个session中对内容![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJzAaaCMmScCC32oicOs2diaFmqa0vv7owIEftxFtf3o7WWdySmEjGW5AA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和内容![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJe11L6DqpgDn4icmtZEjibWA6SF4A1gSI67LKNegkibrCIiaAdicpdvPJHhQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)都产生了点击行为，那么内容![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJdf8GKbic6wW7KW6pqt0HbkyrsjPXdp6G5J4dJyFnswTpgq4tSY7Vb8A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)与内容![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJ2dxjjoc4pFYZZwhoFUj4u0qEsBWtqySjgXtnT6JrXWkKYS5NSdKbww/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)就产生一条边![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJnc0QjIS4dRlEpVViaWtg3TIbYnRAd0QbqKNQWwv9yAcnAocgLzxBDrQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)，权重![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJ05ymEX1xK597JgYYzFnahMlrf8kl5P6JQ7IbuwFDqVvic23YXfiarQbg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)的计算方式为：

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJhwjnuLy3L3mbSmDOpFq6s5gyal30UDB2Kx5Q4BiagAJBYagfwaZu5qg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

其中，![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJFL1YtPpw7PjXUMQndKHc08jAqtma4rYfdbicgMhfWiaicaibYQkr66ZxIw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)为内容![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJzAaaCMmScCC32oicOs2diaFmqa0vv7owIEftxFtf3o7WWdySmEjGW5AA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)与![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJe11L6DqpgDn4icmtZEjibWA6SF4A1gSI67LKNegkibrCIiaAdicpdvPJHhQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)对在所有session中共现次数。



最终，以近15天用户点击日志，每3天为一个session，计算权重矩阵。为了减少噪声点击的影响、加快模型收敛速度，我们设定相关性阈值，仅保留高于该阈值的权重值其他置为0，构建出一张**有向带权**的content-content同构网络图



模型结构

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJAAaS6042jEsEfib9czScpSpF8chpmIpgGJ8vKAnZlxicyecLqWSNhLfw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



具体实现分成了4个模块：

1. Graph Sampling

   借鉴GraphSAGE[5]思想，针对每个节点，根据权重矩阵采样与之相连的topk节点作为邻域得到子图。用采样得到的子图代替原图作为正样本进行训练，并随机采样指定数量的非邻域节点作为负样本。

2. **Encoder**

   考虑到节点相关的特征均为ID类属性特征，我们首先将每个属性特征映射为一个向量，接着将各节点对应的属性特征concat起来，通过一层FC，得到节点的初始向量表征。最后，采用两层邻域聚合得到节点向量表征。在邻域聚合阶段，我们考虑了三种聚合方式：均值(Mean)、求和(Sum)和多头注意力机制(GAT)。综合考虑离线实验效果和模型训练效率，最终采用均值聚合方式。

3. 以Encoder输出的节点向量表征作为输入，重构每个节点对之间的相关性。

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJMUvJPKjdVaJYp9WY1VcCzhzE8eMC30RQfFPia6vFoZxayEVX97A3nwA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

4. 采用binary cross-entropy的分类损失函数进行模型训练，使得重构的正样本对![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJic1Kp1ymmG4ibXxjRLpPeL2CPXxjOe8soZ52jGwqI5e6GgrJ6DOuw9Uw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)高，负样本对![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJdCFeyGnqZxe2ANBx9NPicqwqFfNoV48YcGP9EUdbBuOicMjupWhhneVQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)低。



###### 线上召回

线上服务主要根据用户实时和历史点击内容，进行i2i召回。我们发现直接进行向量召回在轻应用频道业务中会存在如下两个问题：

1. 当线上i2i trigger数量较多时，通过内容ID->拿到embedding->再去召回，会带来rt的大幅上涨。若将embedding聚合成单个向量以降低RT，则会丢失信息；
2. 为避免用户历史点击的热门内容主导用户后续的i2i召回，频道内根据用户交互时间进行trigger selection，即行为越近的点击，重要性越高，对应召回的内容越多。而对向量召回进行trigger selection则较为困难。



因此，我们将向量检索的过程搬到离线环节，预先计算好每条内容相关的topk条候选内容，构建i2i侯选池。



###### 在线效果A/B Test

在推荐效率指标上，pctr+0.80%，ipv_pctr+1.50%，pctrcvr+2.31%。此外，我们观察了推荐在**风格和空间**两个属性上的多样性指标。单次请求中，**曝光不同空间内容数-0.62%，曝光不同风格内容数-0.04%，点击不同空间内容数+0.08%，点击不同风格内容数+0.23%**。从单次曝光上看，**空间和风格多样性的性的指标上并没有取得预期的效果，猜测为内容侧包含丰富的side info且这些信息往往具有不同的分布，以ID特征的方式融合进节点表征，一方面会丢失信息，另一方面会存在归纳偏置，即容易被某个领域信息主导节点表征的学习**。



##### 基于 multi-view 的图表征学习

（多视图）

在第二阶段，我们尝试了基于多任务多视图的图表征学习——**M2GRL**[1]。该模型将大规模异构网络表征学习拆解为**Intra-view task**和**inter-view task**两类任务，前者用于**学习同构图内节点的表征**，后者则用于**学习不同域节点之间的关系**。并以多任务学习缓解不同域信息归纳偏置的问题。



和第一阶段一致，我们采用相同的方式根据用户近15天在每平每屋频道内行为数据构建多个intra-view task的同构网络。inter-view task的网络则根据内容与side info之间的关联关系构建。

**模型结构**

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJTn64ClsHicHcHWmH5sHq3l6fHIR0AclXd7kwY9dia4lEdE09H3uK6Zvg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



首先，除了content-content同构图之外，针对每个view单独构建一个同构网络，如category-category、item-item等。每一个同构网络对应一个Intra-view task，采用同构图表征学习方法得到节点向量表征。接着，**每一个域信息融合到内容向量表征对应一个inter-view task，采用空间对齐的思想完成信息融合**。最终，**使用同方差不确定性来动态调整多任务学习在训练时候的权重**。

###### 线上召回

同样的，这一环节我们离线计算好每条内容相关的topk条候选内容，构建i2i候选池。



###### 在线效果A/B Test

对比2.1技术方案，在推荐效率指标上pctr+0.49%，ipv_pctr+0.22%，pctcvr+0.72%。在多样性指标上，单次请求中，曝光不同空间内容数-0.06%，曝光不同风格内容数+0.14%，点击不同空间内容数+0.83%，点击不同风格内容数+1.24%。此外，单链路曝光和点击的内容篇数分别提升+44.4%和+51.41%。说明M2GRL一定程度上可以缓解归纳偏置，避免部分view的特征主导节点表征的学习。



#### 流行度去偏

通过深度挖掘side info构建更多的关联后，我们发现线上召回的内容依然较为集中。这是因为样本分布不均匀，热门样本数量多，导致训练模型偏向于推荐流行内容，进一步加剧线上的马太效应。因此，我们期望从流行度去偏的角度，挖掘用户真实意图，提高零少行为用户召回结果的准确性和多样性。



**现有的解决流行度去偏问题方法包括：**

1. 根据商品的流行度给样本设计权重
2. 利用一小部分无偏数据辅助学习



然而，这两种方法并未将兴趣和流行度这两种因素解耦，且**忽视了用户受流行度影响的差异性以及内容流行度随时间变化的迁移性**，在鲁棒性和可解释性上均存在一定的局限性。借鉴文章[3]的DICE(Disentangling Interest and Conformity with Causal Embedding)解耦框架，我们将驱动用户行为的兴趣和流行度这两类因素解耦成两套向量表征，学习用户真正的兴趣向量进行内容召回。



##### **DICE框架简介**

DICE是2021年发表在WWW Disentangling Interest and Conformity with Causal Embedding 一文中提出的应用于推荐业务中的基于因果推断逻辑的向量解耦框架。其特点是不同影响因素用各自特定的数据集训练，从而从结构上将用户行为背后的兴趣和流行度驱动因素表征学习解耦，且任意表征学习模型都可以集成。

首先，解耦单一的user和item向量表征为interest和conformity两段向量。其中，interest部分为仅与兴趣因素相关的向量，conformity部分为仅与流行度因素相关的向量。相比较于仅从item角度增加一项标量的方法，这样能够体现用户的特异性。形式化为：



![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJrRFCElXlc1OewDMQamXqyib3rZw430NRyK5tjUiclkZBWics8Z9TfPIeQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



其中，![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJVe4ZiaDicALc5vj4bZUuNtTLeYIDYamHLMJRVL3swL6SIz2ZAjhFjY4A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJqFsT2j63s7BZRhRAXNr9fFqBclFa7uxsgHZC10ZBy0OZPZthBiahgibg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)分别对应user侧解耦出的interest和conformity两部分向量，![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJq1qxJmPZqvHiaFDCibHur9pAlqqT9IK9lTYcgibqV0KIHURYNE0shjxIQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJ6bwaYqQRNdIunYaw5WPHLHO1evu0jdgfmBMWibDul4eW3TghHGYxqFw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)分别对应item侧解耦出的interest和conformity两部分向量。![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJtpGShN8UWvRVsamkscmSyQCknMyhiczyOBapv3cfElHhjL25MlF98sQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)为兴趣因素驱动分，![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJYaZDzDw6ibicktEaIXnNNDrdKLIGfxmdXgibdqD0iaiaeeedNb8icobYYCibQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)为流行度因素驱动分。最终通过加法建模user对item的点击行为![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJCmUObOqrKygicPhTbsIFQC3PwlKJ6ne2tgT7PWQUd32NOGzAl7V2P2A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)。



其次，利用因果推断产生的碰撞效将点击行为建模的等式扩展为不等式从而获得特定原因的数据，并以此作为监督信号来引导两部分向量表征的解耦。具体地，根据正负样本的流行度拆分数据集为![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJible8X3gHehew2bJnkwicvbWe6ibFRohfrI8ZV9X0SQ43EclhEab1pCTw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJicnYRnQWbk1SFL6uIbo1B3JIS44mQHTibkUpsUeD8PDvoSK9O6fKwEZw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)两部分：



1. ![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJible8X3gHehew2bJnkwicvbWe6ibFRohfrI8ZV9X0SQ43EclhEab1pCTw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)：负样本的流行程度低于正样本，那么猜测用户群体更多地是受流行度因素驱动产生的点击行为。
2. ![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJicnYRnQWbk1SFL6uIbo1B3JIS44mQHTibkUpsUeD8PDvoSK9O6fKwEZw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)：负样本的流行程度高于正样本，那么猜测用户群体更多地是受兴趣因素驱动产生的点击行为。



![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8n78Qtx6IibRicpZBW9IePhJlqUSd8iafQpEvknYV5xg5zX7qCYHTWhM7g5YxjQ1Dd9SAgkqQyTN5LA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



其中，![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlpz9AnxWx1s53Rqph9upJDWPZdE7gfKlwiadKJaGYZMsrd3kO8EnQMeQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlTIKjfibeE09KOWf2YXGK1javWIxJd6KmUNcibJicZI5XjuibY1XQo3wdMQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)分别是兴趣驱动分矩阵和流行度驱动分矩阵。

接着，采用多任务学习以综合和平衡兴趣和流行度两个点击行为驱动因素。如下图所示，拆解解耦表征学习为**Conformity Modeling、Interest Modeling、Estimating Clicks和Discrepancy Task**四个任务。

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlq40PDrzZQEfiapW9iaFhc1L3SJ8xuSxUQ2Cgpw9hRvlgy1GQdUJTz48g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

假设函数![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlZIYGEcPrGPcCyK1sQmL30iaicVoxzPXKiccExJJpDfAS1cccjKciaXRiagg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)为任意pairwise损失函数，输入包含userID、正样本itemID和负样本itemID的三元组![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlZvibUkAjt2y0DwwwicnTdN3sRVjajQJ7jEUT3iaWuDknYfYzCGp31eb5g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)，那么上述四个任务损失函数可以定义为：



1. Conformity Modeling：基于![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlWqSicicgnxQsQcwL2DOLAwoOTQCkqH0VWzJ9LzU3JsbQqxcCN3nOFhvQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl6icwOHJp13CQbBdv1qGwmKzsgk7HeLjkDU86W6OosLaXFttYypNPJcw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)数据集建模流行度因素驱动分。

   ![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlwN0BcichDDz6y7F2icuOP2AyFBXy86eclTere039UXbmNOvDcflftzpA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

2. Interest Modeling：基于![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl6icwOHJp13CQbBdv1qGwmKzsgk7HeLjkDU86W6OosLaXFttYypNPJcw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)数据集建模兴趣因素驱动分。

   ![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl9lkbckuQZ5JfQnvicLePsoxBdludbYLXxKotPazHByGAl7Nrrz75K5A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

3. Estimating Clicks：基于![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlWqSicicgnxQsQcwL2DOLAwoOTQCkqH0VWzJ9LzU3JsbQqxcCN3nOFhvQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)和![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl6icwOHJp13CQbBdv1qGwmKzsgk7HeLjkDU86W6OosLaXFttYypNPJcw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)数据集建模点击行为。

   ![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlw1mSdNQf6Dt45CTqgC8zyuKL1U9Y8p0bOzwFNRjn9DSCfdfzhPSJLA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

4. Discrepancy Task：利用一阶范数约束interest和conformity两部分向量要尽可能不相关。

   ![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl81jxbMl5Rtz07j3eQ2cJayWNict8D64gy5mfZwlPiaNwzSHd2gcywF9A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



最后，通过不同的参数将四种task结合起来。

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl3WjYNL7ibcSpfyelrxfBDkPpFD8FIfLctvLdR08ouYArcrwGdVvqHjg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)





##### **基于DICE解耦框架的图表征学习**

针对零少行为用户，我们根据用户**人口属性**特征和进入**频道的引流信息**将用户分为不同的群体![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXleLPRMVTyN4N6UosAq78muiasoAskd6MMbwP6hicHRu8THUiaj82nAYg4w/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)，基于群体在每平每屋频道内行为数据构建![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlytVHaBek058b2hnKuRIueicMLEoyYrtWMibZqCWf6ytOpLsNPcly8HUQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)二部图网络，并将其输入到兴趣和流行度解耦模型中，进行流行度去偏，学习不同用户群体真实意图。其中，边的权重![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlYLVXtqaJJ05QrAcH8ZSQRD0yZf3xtDGiaaLNr5HGPBFoHlc7p8F0Heg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)的计算方式为：

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlFCQa1HQlia9hRZ9ZM5Yzya3Hib7jFIqT6dSox5rqyNWibsBhHneHiasO7A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

其中，![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXll12TPOIicLSH3nzkIC23nI4PvDA0m2TBJP2SF7SecqRUDRzzuUkyXhg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)为用户群体ut![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlwgxrnUc3lOXDdK0oOhVicibiazQmuKG5oEFoTiaCvqvjOxJEKB6uEL5s1A/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)对内容item![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl4UMV3SpichghxA0DDTAscvBglYes1mImWFz0MOT3kzxGtLIZp4W7G2w/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)的点击次数。

最终，以近15天用户点击日志，计算权重矩阵。为了减少用户群体行为中噪声点击的影响，我们以设定相关性阈值仅保留高于该阈值的权重值其它置为0的方式，构建出一张带权的![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlZ7Uaq5qyWb0YiaqOkCf3lOkiaIl2ibkcAribstYseUDgMjia7AEFib1ZAvtg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)网络图。



此外，与之前工作类似，我们也将内容挂载的商品、类目、风格、空间等side info作为节点的属性特征，融合进图的节点表征过程中，并引入item-item相似度网络，进一步丰富网络数据。



##### 模型结构

如下图所示，我们基于两个**二部图的GNN模型**分别学习user和item的interest和conformity两部分向量，并集成到DICE框架中，将用户交互行为产生的驱动因素解耦为**兴趣和流行度**两个部分。

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXl8yHTpkgvkibMN8VvPoOtCT90SdeicbJEyEOGPyCAkrnEjhbwS08Okzjw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



与原文的排序模型不同，仅仅使用曝光未点击作为召回模型的负样本是不够的，有时甚至会导致负向的效果。这是因为召回模型面对的整个内容侯选池，那么若以线上推荐链路筛选出的用户“最喜欢”内容中的负样本训练召回模型，是无法很好地对全量内容池做出判别的。随机负采样可以让负样本尽量符合线上召回环节的样本真实分布。然而随机采样得到的样本，很多情况下和正样本差异较大，这就导致模型无法学到有用的细粒度信息。因此，我们还挖掘了**hard negative增强样本**。



##### 可视化示例

随机挑选线上3000条内容可视化对应的interest和conformity向量表征。按曝光pv由低到高，将内容分为4个等级，并用不同的颜色标记出。其中interest向量对应popular 1~4等级，conformity向量对应popular 6~9等级， 结果如下图所示：

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8zZtLVahOicmUqnuRoDulXlwPD04vuzfMjk9LLbUQYvUJqpK2hsBPnN2ibUYwMQrhperNkibiaJicj7xQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



从图中可以发现，两种解耦表征之间存在一条界限，并且左边conformity向量表征按曝光pv的不同，也有一定的分层聚类效果。



##### 线上召回

线上服务主要根据用户属性特征和引流信息，查询的得到**interest向量**进行u2i召回。



##### 在线效果A/B Test

在推荐效率指标上pctr+0.75%，ipv_pctr+0.48%，pctcvr+1.24%。在多样性指标上，单次请求中，曝光不同空间内容数+0.04%，曝光不同风格内容数+0.15%，点击不同空间内容数+0.68%，点击不同风格内容数+0.72%。从中可以发现**通过向量解耦解决流行度偏差问题，挖掘用户真实意图，对于效率指标和多样性指标的提升均有一定的帮助**。



#### 总结与展望

大部分淘内轻应用频道新用户占比高，用户行为稀疏且内容分发头部曝光现象严重。保证召回结果准确性的同时提高发散性是推荐系统优化的重点之一，本文主要介绍了基于GNN模型在每平每屋轻应用频道内容召回上的实践。



1. 基于链接的传递性建模高阶关系，同时融合multi-view side info以建立起更多的关联，缓解行为数据稀疏性问题。
2. 基于DICE向量解耦框架，对用户交互行为中的流行度偏差去偏，挖掘用户泛群体的真实兴趣意图，以期缓解零少行为用户推荐马太效应。



​	未来在GNN模型的探索上，我们会考虑优化长尾问题，避免**high-degree的节点主导了整个模型的训练，导致low-degree的节点得不到充分的学习**，也会尝试将GNN与排序模型结合起来，进一步缓解数据稀疏性问题。此外，**除了兴趣与流行度之外，用户交互行为可以是由更多的潜在因素共同驱动，例如风格、款式偏好等**。近年来学术界和业内也**有不少工作尝试将图像领域的解耦表征模型应用到推荐业务**中，让模型能够构建更多的关联，从人的角度学习得到**更为鲁棒、更具可解释性的推荐模式**，因此**细粒度的GNN解耦表征**也是我们后续想要持续深挖的一个方向。













