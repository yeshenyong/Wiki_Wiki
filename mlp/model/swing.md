# 召回 swing 算法

摘自：https://www.jianshu.com/p/a5d46cdc2b4e



基于图结构的实时推荐算法 Swing，能够计算 item-item 之间的相似性。Swing 指的是秋千，用户和物品的二部图中会存在很多这种秋千，例如 (u1,u2,i1), 即用户 1 和 2 都购买过物品 i，三者构成一个秋千 (三角形缺一条边)。这实际上是 3 阶交互关系。传统的启发式近邻方法只关注用户和物品之间的二阶交互关系。Swing 会关注这种 3 阶关系。这种方法的一个直觉来源于，如果多个 user 在点击了 i1 的同时，都只共同点了某一个其他的 i2，那么 i1 和 i2 一定是强关联的，这种未知的强关联关系相当于是通过用户来传递的。另一方面，如果两个 user pair 对之间构成的 swing 结构越多，则每个结构越弱，在这个 pair 对上每个节点分到的权重越低。公式如下：



![img](https:////upload-images.jianshu.io/upload_images/18705940-b795da1d87fb5e28.png?imageMogr2/auto-orient/strip|imageView2/2/w/381/format/webp)



为了衡量物品 i 和 j 的相似性，考察都购买了物品 i 和 j 的用户 u 和 v， 如果这两个用户共同购买的物品越少，则物品 i 和 j 的相似性越高。极端情况下，两个用户都购买了某个物品，且两个用户所有购买的物品中，共同购买的物品只有这两个，说明这两个用户兴趣差异非常大，然而却同时购买了这两个物品，则说明这两个物品相似性非常大！

**思考：为何swing 算法的召回结果会更好**
 1：svd, CF 等算法在对对用户行为进行一层抽象
 2：打分近似， 矩阵近似计算
 3：svd, CF数据稀疏问题
 4：swing 算法基于二步图，对用户和商品行为进行了直接建模，其原理和思想更加贴近实际用户特征

关键代码



![img](https:////upload-images.jianshu.io/upload_images/18705940-b61b1e8e87c59698.png?imageMogr2/auto-orient/strip|imageView2/2/w/1014/format/webp)

```python
def SwingRecall(u2items):
    u2Swing = defaultdict(lambda:dict())
    for u in u2items:
        wu = pow(len(u2items[u])+5,-0.35)
        for v in u2items:
            if v == u:
                continue
            wv = wu*pow(len(u2items[v])+5,-0.35)
            inter_items = set(u2items[u]).intersection(set(u2items[v]))
            for i in inter_items:
                for j in inter_items:
                    if j==i:
                        continue
                    if j not in u2Swing[i]:
                        u2Swing[i][j] = 0
                    u2Swing[i][j] += wv/(1+len(inter_items))
         break
    return u2Swing
```























