# DSSM listwise pairwise 利用全埋点数据

可以朝listwise，pairwise，pointwise的视角去看。

DSSM的loss希望是拉大正样本和一堆负样本的差距，本质上是只要保证正负样本rank有序。交叉熵二分类的loss希望是区分哪些是正样本，哪些是负样本，希望将分类划分好，这种问题会比上面的rank问题更难。

具体应用的得看具体场景，在搜索推荐中，往往都是rank问题，用dssm这种loss，甚至用listwise loss效果会更好

## 初级理念

pairwise 的排序算法用于推荐系统的排序任务中为什么效果差于pointwise的ctr？

answer by：https://www.zhihu.com/question/338044033/answer/1825537440

观点一：

如何实现pairwise，listwise。我也曾经想过这个问题，很少有人给出listwise样本的构造方式。所以在这里给出一个我曾经在工程上使用的，个人感觉比较实用listwise的小技巧。其实listwise在工业界还是比较难通过标注来获得样本的，所以我们退而求其次，不构造严格的listwise样本，转而求list里top1 概率，经典的DSSM论文就是这个思想。 

top1 概率的参考文献《Learning to Rank: From Pairwise Approach to Listwise Approach》。相当于 一条样本是“query doc+ doc- doc- doc- .....”,令query和doc+概率最大，等价于query和其他doc-概率最小，而不是强行给query和N个doc排序

1）因为正样本很容易构造，负样本可以在batch内动态随机负采样，这样每个epoch都能动态学习N个新的样本。谷歌有一篇论文的双塔召回 ，就是采取的这个策略。   

2）loss的设计。个人觉得不太适合focal loss，确实也尝试过，场景中doc量级千万，query50W，效果没提升。因为只要是随机负采样，都有可能误把正样本选为负样本，假如对某个正样本，我们进行10次负采样，每次采5个负样本，即使9次负采样的5个负样本都采对了，但是第10次采错了一个，focal loss 会加大第10次的惩罚，从而导致梯度相对于前9次变化巨大，白白更新了前9次的梯度。所以可以使用简单的softmax后，对top1交叉熵即可

观点二：
（1）搜索是带 query 的、有意识的被动推荐，对于搜索而言，相关性是及其重要的事情。query 限制了你召回商品相关性，比如 “ONLY 连衣裙”，召回回来一批相似性极高的连衣裙，同时用户心智也决定了他将高度关注商品之间的细微差别，比如价格、款式等，因此这些商品才有必要比个高下。

（2）推荐是发散的、无意识的主动推荐，相比搜索而言，准确性不再是第一要务（想象下因为你点过一些女装给你出一整屏的连衣裙的感觉），多样性是一个必要的指标，这导致了推荐结果极其发散。用户对推荐结果多样性的心智使得他不关注两个商品之间的比较，对于算法而言不再关注商品之间两两的比较，我只要每个都预测准了，反正最后也要打散的。而且多样性也导致了推荐场景没有像搜索一样适合做 pairwise 的样本

观点三：（较为简单化）

（1）搜索是带 query 的、有意识的被动推荐，对于搜索而言，相关性是及其重要的事情。query 限制了你召回商品相关性，比如 “ONLY 连衣裙”，召回回来一批相似性极高的连衣裙，同时用户心智也决定了他将高度关注商品之间的细微差别，比如价格、款式等，因此这些商品才有必要比个高下。

（2）推荐是发散的、无意识的主动推荐，相比搜索而言，准确性不再是第一要务（想象下因为你点过一些女装给你出一整屏的连衣裙的感觉），多样性是一个必要的指标，这导致了推荐结果极其发散。用户对推荐结果多样性的心智使得他不关注两个商品之间的比较，对于算法而言不再关注商品之间两两的比较，我只要每个都预测准了，反正最后也要打散的。而且多样性也导致了推荐场景没有像搜索一样适合做 pairwise 的样本

实践简略知识

本文实现的listwise loss目前应用于基于ListwWise的召回模型中，在召回中，一般分为用户侧和item侧，模型最终分别输出user_vector和item_vector，在pointwise模型中，这两个vector的shape都为（batch_size,dim）,直接求两者的内积或余弦，然后过sigmoid，最后采用交叉熵获得loss进行反向传播。而在listwise模型中，item_vector的shape变为（batch_size,list_size,dim）其中，list_size 为序的长度。这样，在listwise模型中，user_vector与item_vector求内积后，得到的结果的shape为（batch_size，list_size）,而输入的label也是(batch_size,list_size)，所以有了输出结果与label，据此可以设计listwise loss



## 业界实践

### PairWise

1. 精排打分头部与尾部组成pair
2. 精排分段组成pair
3. 正样本与精排参竞未展现组pair



### ListWise

1. 对精排Top 400 的序，取 N 段，每段采样一个 item，将 N 个 item 及其精排分组成list 作为样本，通过listwise loss（KL 散度等），使粗排对 N 个item 的序尽可能接近精排，提升面向后链路一致性