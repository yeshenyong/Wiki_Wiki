# RankFlow: 多阶段级联排序系统联合优化

[RankFlow: 多阶段级联排序系统联合优化 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/643741144)

**step1 ：self-learning**

每个阶段模型先通过point-wise方式自学一遍训练数据中蕴含的信息

![img](https://pic1.zhimg.com/80/v2-7385929b945c8ec2ca1d04e9f01aae60_1440w.webp)

**step2 : tutor-learning**

根据下游模型的指导进一步提高学习效果，具体向下游模型学习以下两点：

- 样本间的相对排序ranking_loss，根据下游模型打分确定样本pair的相对排序。
- 样本打分绝对准确度mse_loss，尽量拟合下游模型打分绝对值。

![img](https://pic3.zhimg.com/80/v2-e2828e379c98842aac60acdd81954406_1440w.webp)