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



