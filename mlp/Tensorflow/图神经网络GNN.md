# 图神经网络GNN

## Graph Embedding

### LINE

LINE - 大规模的图上，表示节点之间的结构信息

一阶：局部的结构信息

二阶：节点的邻居。共享另据的节点可能是相似的



DeepWalk 在无向图上，LINE在有向图上使用



根据图（数据集）的性质，决定最佳的算法，选用最佳的图信息截取



### node2vec

有策略进行随机游走

![image-20211125215704217](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211125215704217.png)

homophily：同质性

structural equivalence：结构等价性



DFS，即q值小，探索强。会捕获homophily同质性节点，即相邻节点表示类似



BFS，即p值小，保守周围。会捕获结构性，即某些节点的图上结构类似





### Struc2vec

捕获结构相似性

适用于节点分类中，其结构标识比邻居标识更重要是。采用struct2vec最好



### SDNE

structural deep network embedding



### 总结

1. deepwalk：采用随机游走，形成序列，采用skip-gram方式生成节点embedding
2. node2vec：不同的随机游走策略，形成序列，类似skip-gram方式生成embedding
3. LINE：捕获节点的一阶和二阶相似度，分别求解，再将一阶二阶拼接在一起，作为节点的embedding
4. struc2vec：对图的结构信息进行捕获，在其结构重要性大于邻居重要性时，有较好的效果
5. SDNE：采用了多个非线性层的方式捕获一阶二阶的相似性





## Graph Neural Network

### GCN（不是太了解）

图卷积神经网络

需要记录整张图的邻居信息再进行训练



### LSTM

long short -term memory

长短期记忆——RNN形式之一



### GraphSAGE

采样和聚合

把邻居信息聚合到一起

将可学习的w参数，与src_node 和 邻居信息聚合的拼接到一起形成新的

![image-20211127180452465](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211127180452465.png)

agg_type : mean

采样：不是对所有邻居信息都去聚合，是采样固定或随机的对象。

所采样本m个，一共样本n个（邻居）

m < n，随机or按出入度采样

m > n，重复采样



S1：第一层采样的邻居数量

S2：第二层采样的邻居数量

S1 * S2 <= 500 数量是比较好的 K = 2时候



**聚合函数**：对称的，对于输入排列不变的

![image-20211128103625005](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211128103625005.png)



![image-20211128103654392](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211128103654392.png)



#### 问题

- 标签信息多值如何处理（非普通的分类器）
- 根据入度权重采样如何处理
- 标签信息如何特征化





### GAT

GAT 通常一种说法就是加权求和

![image-20211128104419570](C:\Users\10505\AppData\Roaming\Typora\typora-user-images\image-20211128104419570.png)



### 应用



图网络应用

1. GCN，GraphSAGE，GAT，节点的半监督学习
2. GraphSAGE，GAT的推理学习任务









## 批标准化

batch normalization

























