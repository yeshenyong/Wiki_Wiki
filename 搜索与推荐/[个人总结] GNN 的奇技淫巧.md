# GNN 的奇技淫巧





### 正样本

1. We also experimented with different ways of mining positives and have interesting findings as follows:（来自Facebook 的EBR 论文）
   - clicks（点击）: it is intuitive to use clicked results as positives, since clicks indicates users’ feedback of the result being a likely match to users’ search intent. 
   - impressions: the idea is that we treat retrieval as an approximation to ranker but can execute fast. Thereafter, **we want to design the retrieval model to learn to return the same set of results that will be ranked high by the ranker**. In this sense, all results shown or impressed to the users are equally positive for retrieval model learning（Facebook 上没效果，但是在稀疏的场景下个人觉得的值得一试）
2. 一个session内 用户的点击的同类目（item 和  item）加边
3. 一个session内 用户的点击的（item 和 item）加边
4. 



### 负样本

1. random samples: for each query, we randomly sample documents from the document pool as negatives. 

2. non-click impressions: for each query, we randomly sample those impressed but not clicked results in the same session as negatives（展示未点击，会有SSB 样本选择偏差问题，不太建议，也可以尝试）

3. **本文与百度Mobius的作法，二者的作法极其相似，都是用上一版本的召回模型筛选出"没那么相似"的<user,doc>对，作为额外负样本，训练下一版本召回模型**

   - 怎么定义“没那么相似”？文章中是拿召回位置在101~500上的物料。排名太靠前那是正样本，不能用；太靠后，与随机无异，也不能用；只能取中段。

   - 上一个版本的召回模型作用于哪一个候选集上？文章中提供了online和offline两个版本。online时就是一个batch中所有user与所有d+的cross join，这一点就与Mobius几乎一模一样了。

   - offline的时候，需要拿上一版本的召回模型过一遍历史数据，候选集太大，需要用到基于FAISS的ANN。

   - 可能有人还有疑问，这样选择出来的hard negative已经被当前模型判断为“没那么相似”了，那拿它们作为负样本训练模型，还能提供额外信息吗？能起到改善模型的作用吗？

     - 我觉得，一来，这是一个“量变”变“质变”的过程。在上一版召回模型中，这批样本只是“相似度”比较靠后而已；而在训练新模型时，直接划为负样本，从“人民内部矛盾”升级为"敌我矛盾"，能够迫使模型进一步与这部分hard negative“划清界限”
     - 二来，毕竟是百度和Facebook两家团队背书过的方案，还是值得一试。

     不过需要特别强调的是， **hard negative并非要替代easy negative，而是easy negative的补充。在数量上，负样本还是以easy negative为主，文章中经验是将比例维持在easy:hard=100:1** 。毕竟线上召回时，库里绝大多数的物料是与用户八杆子打不着的easy negative，保证easy negative的数量优势，才能hold住模型的及格线

4. 



