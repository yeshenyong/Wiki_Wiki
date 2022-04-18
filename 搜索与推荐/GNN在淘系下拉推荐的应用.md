# GNN 在下拉推荐的应用

转载：https://mp.weixin.qq.com/s/wNnFwiOehZCeFLN7eNuh8w

工作背景：

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8FmrJwfDvZzickY3HaEzKibyOCs7haQpogiaKX3q2PcgO6ZR879oibPCxrickVGXPkU5V2ZQiarxVep3yA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

通过`query` 提高用户的检索程度

**最初下拉模型只使用了长度为10的Item序列，后来扩展到长度为50的Query和Item序列**，直观上看，将序列加长可以包含更丰富的用户历史行为，但是也存在着以下几点问题：

1. 加长序列无法作用于历史行为稀疏的不活跃用户
2. 加长序列对曝光次数较少的长尾`Query` 效果较弱
3. 加长序列带来的收益与巨大的存储效果之比不高



解决方法

问题一：

**GNN对不活跃用户有明显效果。**模型在不同活跃度用户上的AUC对比见**图 7.1**所示，可以有以下几点发现：



1. base_50在低活用户上效果与base_20几乎持平，但是GNN在低活用户上效果却有显著提升。
2. 即使在base_50表现优异的高活用户上，GNN在只使用长20的序列的基础上，效果也比base_50略好。



问题二：

**GNN对长尾Query也有明显效果。**模型在不同PV的Query上的AUC见**图 7.2**所示，可以发现在**长尾Query上GNN相对Base_20和Base_50均有所提升，在Base_50表现优异的高频Query上GNN也能够保持持平。**



问题三：

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8FmrJwfDvZzickY3HaEzKibyuwU0yF9KbP1IzsMeHEZzFsN6CfTWJDMVP2F4EEVcGicPnVNmqic3EWBg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**GNN可以用更少的存储获得更好的效果**。模型使用的训练数据存储消耗与取得的AUC的对比见图 7.3所示，其中*表示只使用了邻居结点的ID特征和时间特征，此处HGCN和Attention都只使用了1阶q2q和i2i邻居。可以看到，**在存储量只有Base_50的60%情况下，GNN却能取得比Base_50略好的效果；在存储量只有Base_50的80%的情况下，GNN相对Base_20可以取得千分之4的提升，相对Base_50可以取得千分之1.5的提升。**





​	一方面，加长序列所带来的收益具有**边际效应**，通过可视化Attention值也能发现，序列中的有效成分主要集中在前20

![图片](https://mmbiz.qpic.cn/mmbiz_png/33P2FdAnju8FmrJwfDvZzickY3HaEzKibyRYyqocuktQrwQYqREZYlP71um9MWGWsPzeedBl36761nDJXBLyb8gg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



**总的来说，针对上述三个问题，我们引入邻居信息，用图神经网络建模，期望在更小的存储消耗下更有效地拓展序列，并对不活跃用户和长尾Query有较好的效果。同时结合实验分析归纳出一些GNN在下拉场景的一般性的结论。**全文按照实习工作的推进顺序组织。



### 前期调研

下拉场景的数据具有两个特点，一是**数据规模庞大**：每天有亿级的用户规模和亿级的样本规模；下拉底池Query的数量级为千万级。二是**数据类型复杂**：用户行为有点击、加购、成交等；行为对象有Query（查询）、Item（商品）等。针对这两个特点，在图神经网络方面着重调研了**大规模和异构性**两个方向，在搜推的应用方面着重调研了工业界的一些工作。

#### 图神经网络的研究方面





### 初步方案

**结合方式**

如何将图神经网络与下拉排序模型结合是一个问题，有两种可行的方式：

- **图神经网络部分单独预训练**



得到的Query和Item Embedding以初始化或Concat的方式作为补充信息辅助排序模型的训练。但是经过咨询了解到，之前预训练的方式几乎没有效果，下拉模型使用的Embedding也都是端到端训练得到的。一个可能的原因是，端到端训练得到的Embedding与目标任务更加相关。



- **将GNN嵌入排序模型端到端训练。**

  

GNN作为大模型中的一个前置模块，为序列中的Query和Item学习Graph Context信息作为补充，在CTR预估目标下进行端到端的训练。最终决定采用这种方式。



**图的构建**

















































































