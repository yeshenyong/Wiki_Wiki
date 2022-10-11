# FM 模型

[FM（Factorization Machines）的理论与实践 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/50426292)

https://datawhalechina.github.io/fun-rec/#/ch02/ch2.1/ch2.1.2/FM



核心点：（学习参数：*w*0,*wi*∈R 均表示需要学习的参数，特征隐向量可以是学习好的也可以一起学习）

![img](https://pic3.zhimg.com/80/v2-1d9b69e9c56206b332328ef5959ff8ee_720w.webp)



特征交叉降维

按照训练样本为单位，逐条学习



`MatchScore` 也有两种

1. 基于原公式进行计算
2. 基于embedding 的内积形式（相关性没那么强）



用户向量：
$$
Vuser=[1;u∈U∑vuxu]
$$


- 用户向量由两项表达式拼接得到。
- 第一项为常数 1，第二项是将用户相关的特征向量进行 sum pooling 。

物品向量：（同样拼接而得）

- 第一项表示物品相关特征向量的一阶、二阶特征交互。
- 第二项是将物品相关的特征向量进行 sum pooling 。



1. 为什么不直接将 FM 中学习到的 User Embedding、 Item Embedding

   答：这样做，也不是不行，但是效果不是特别好。**因为用户喜欢的，未必一定是与自身最匹配的，也包括一些自身性质极佳的item（e.g.,热门item）**，所以，**非常有必要将"所有Item特征一阶权重之和"和“所有Item特征隐向量两两点积之和”考虑进去**，但是也还必须写成点积的形式。