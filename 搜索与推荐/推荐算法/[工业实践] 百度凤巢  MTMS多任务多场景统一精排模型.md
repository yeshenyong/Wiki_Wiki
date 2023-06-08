# 百度凤巢 | MTMS多任务多场景统一精排模型

转自：[百度凤巢 | MTMS多任务多场景统一精排模型 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/617002509)

## **Motivation**

文章研究多场景多任务问题的核心动机和大部分工作类似：用户在多个场景可能有相同的兴趣信号，因此理想情况下排序模型应该是一种多场景的范式，这样能够充分利用多场景的数据，**比如百度搜索、新闻feeds、视频等，也可以缓解部分场景数据过于稀疏问题**。除此之外，文章还比较关注场景的拓展性，新引入的场景要能够快速接入现有的精排模型，不影响已有场景的建模效果。对于多任务，动机也主要是考虑**目标之间的依赖性、数据稀疏性，希望能够缓解多任务的跷跷板问题**。

总体来说，构建统一的多场景多任务精排模型存在两大挑战：

- 不同目标之间**数据不平衡**。比如CTR和CVR，如果同时训练这俩目标，模型很可能偏向于CTR，导致CVR的指标受损。
- **embedding组件耦合**。多任务或多场景共享embedding是常见的范式，好处是能够缓解部分场景的数据稀疏性问题，坏处是模型不同组件之间耦合在一起，拓展性差且容易有跷跷板问题。尤其当场景的数量很多的时候，这个问题更为严重。比如百度可能有超过100个场景。

为了解决这样的问题，作者提出了一种面向多任务多场景的统一精排模型。文章主要的贡献有2个部分：

- 提出了一种面向多任务多场景的统一精排模型MTMS(**Multi-Task and Multi-Scene uniﬁed ranking model**)，为每个场景每个目标设置独立的表征(**independent/non-shared embedding**)。不同组件之间耦合度低，对于场景或任务的扩展性好。
- 提出了一种交替更新的模型训练策略。embedding update时关注表征的学习，fine tuning时固定住表征，只更新排序上层的网络参数。

## **Phoenix Nest: Ads Ranking**

在介绍方案之间，我们可以先一睹百度凤巢(Phoenix Nest)在线广告ranking的发展，业务范畴涵盖百度搜索广告和百度Feeds广告：

- 2010年，MPI-CPU-based 分布式参数服务器训练超大规模CTR模型，以logistic regression为主；
- 2013年，发布第一代深度广告ranking模型，尽管深度模型是主流，但仍然有很多业务在使用非深度模型，效果也很好[3]；
- 2019年，MOBIUS，研究ANN向量召回的工作[4]；
- 2020年，升级为GPU集群参数服务器(GPU cluster parameter servers)替代了MPI-CPU架构[2]；
- 2021年，在GPU架构的基础上，研究模型量化，能够在不增加存储成本的前提下增大embedding table，使得模型更大，提升预估准确性[5]；
- 2021年，提出了GemNN，一种基于门控的多目标网络，能够从粗到精地预估CTR，逐渐减少广告候选集，并允许从上游到下游任务共享参数，以提高训练效率；
- 2021年，MTMS，也就是本文的工作，多任务多场景建模范式，并提出join-update两阶段训练范式。

除此之外，也研究了一些领域特定的广告算法，比如：图像-广告相关性[7]、多模态[8]、视频广告[9]等。



## **Solution**

TL;DR，MTMS模型整体架构是在ESMM基础上，对embedding层和模型训练方式做了一些改进：

- **Embedding层**：ESMM在多任务上共享Embedding、本方案不同场景不同任务独立使用独立的Embedding；

- **训练方式**：ESMM直接端到端训练，MTMS提出了两阶段训练范式 (据说是凤巢祖传Join-Update)。[2]：

- - **learning representation阶段**：不同场景、不同任务(CTR/CVR)不共享Embedding、各自**独立地训练各自的embedding到收敛**；
  - **fine-tuning 阶段**：learning representation阶段得到的不同任务的Embedding join拼接在一起并保持固定(frozen)，**只更新MLP参数**，不同场景不同任务的embedding**本身参数保持固定**，但会参与到**各个场景和各个任务**的MLP tower的学习，充分利用各个场景各个任务学习到的表征信息。

- **损失函数**：采用传统的方法，直接建模CTR, CVR损失；而不是将CVR作为中间变量；此外会加入CTCVR的辅助目标。

## **Network Structure**

下面以百度feeds推荐和搜索场景联合建模为例来介绍MTMS模型。

![img](https://pic2.zhimg.com/80/v2-e2d9fd8680a65f9fd1fafbfd4b7bc1bd_1440w.webp)

- 多目标：每个场景都是CTR和CVR两个目标。
- 样本：CTR用的曝光→点击数据来训练；和ESMM不同的是，CVR是用点击→转化的样本直接构建目标训练的。
- 表征层：每个场景的CTR和CVR都各自独立地进行表征。
- 预测层：多个场景的表征向量会拼接在一起，用于各个场景不同任务的finetune和predict过程(下文会介绍)。比如：user_id得到的feeds场景ctr表征、feeds场景cvr表征、搜索场景ctr表征、搜索场景cvr表征，可以全部拼接聚合起来，分别用于feeds场景的ctr任务finetune和predict、cvr任务finetune和predict；用于搜索场景的ctr任务finetune和predict、cvr任务finetune和predict。
- Loss：ESMM中，CVR是作为中间变量，不直接构建loss。通常来说，当CVR的样本非常少并且CVR的AUC值较小时，ESMM这种CVR作为中间变量来构建loss的做法是合理的，能够缓解稀疏性问题。当CVR的样本比较丰富并且CVR的AUC值较大时，CTR任务会影响CVR的预估，也就是说这种时候ESMM做法不合适。百度广告cvr样本较为充足，因此选择了单独直接建模CVR的方式。具体而言：
  L_MTMS = L_ctr + L_cvr + L_ctcvr

- 可以看到除了CTR和CVR损失，还多了一个辅助损失CTCVR，是基于ctr和cvr的乘积来得到的预测值。

## **Training**

本部分介绍下上述的模型是如何训练的。作者采取了交替训练的过程，也是本文最精彩的部分，如下图(b)和(c)。

这个部分有挺多我个人读paper后的理解，不一定对，有错误的地方欢迎指出。

![img](https://pic3.zhimg.com/80/v2-f6a132cc75f9b7846906130a4c81c77a_1440w.webp)

### **Embedding Update**

在time slot T，会使用T−1的表征来做初始化，然后用T时候的样本做训练。使用(b)图来训练和更新模型，一个场景内的CTR和CVR的表征不共享，每个场景之间也不共享。可以理解为**每个场景、每个任务都构建了1个子模型分别做训练直到收敛**，只不过实现的时候做一些Embedding Table等隔离操作，可以写在一份模型代码中。这样2个场景2个任务，就相当于有4个模型，可以**并行优化**，按照ESMM的结构来训练直到收敛，训练的时候embedding层和task tower都是会更新的。除此之外注意还有个CTCVR Task，会在CTR和CVR收敛后，再训练CTCVR损失直到收敛，这样CTR embedding和CVR embedding都会被更新，2个场景可并行执行各自的CTCVR训练过程。即使场景再多，也可以在1个服务内部做隔离，并行训练



### **Ranking Network Fine Tune**

Finetune步骤的结构如图(c)所示。上述Update步骤得到了各个场景各个任务训练收敛的embedding，在finetune步骤中，我们会固定这些embedding，只对task tower进行finetune，也即对每个场景每个任务的输出tower层做微调(upper DNN)。相当于所有预训练好的embedding都frozen并拼接(join)在一起，作为各个场景各个任务tower DNN的输入，用于DNN的finetune，这样就能够充分利用各个场景各个任务的知识了，实现不同场景不同任务之间的沟通。

比如同一个user_id，在不同场景下都训练得到了1个表征，那么在finetune的时候，对某个场景的某个任务，该user_id所有场景的向量可以一起参与finetune和predict，这样就利用到了**其他场景的信息**了。

这种方式的拓展性很好，当加入1个场景时，独立对该场景执行embedding update的过程，不会影响其他场景的embedding。当进行finetune时，把该场景的embedding join到其他场景embedding list中，再作为DNN tower的输入，用于微调各个场景各个任务的tower参数。

最后想提一嘴Unified Feature Management，文中描述的不多，但我个人理解对于实践来说这部分也挺重要的，应该也有一些实现上的细节，感兴趣的可以私下讨论。

## **Evaluation**

- 评估指标：离线：AUC，MAE；在线：CVR, CTR, t_charge(=bid_price x CVR)。
- 数据集：650M的广告曝光数据，14M的点击数据，2020.11.1~2021.2.15，用了3.5个月的数据来训练。
- 过去3个月的历史数据来做模型的热启动；接着在最近3天的数据上继续训练，再在测试集上评估，天粒度的评估。
- 如果离线指标不错，会上线做A/B测试，并逐渐调大流量。(比如：30%→50%→90%)

## **Loss实验**

和ESMM不同的是，作者直接建模了CVR Loss，因此需要做个对比实验看下优势。

![img](https://pic4.zhimg.com/80/v2-886f3a96a7191f5664d5a4511c6c5bbf_1440w.webp)

CTR+CVR+CTCVR且不共享embedding的方式效果最好。作者的结论是如果AUC的水平比较高，比如0.8以上，那么显式地建模CVR是会更好的。

## **对比实验**

作者采用的baseline是CTR和CVR的，也就是上述Loss实验中的第一个，因为在百度场景，该baseline比ESMM还好不少。

在此基础上做了单场景多目标的实验以及多场景多目标的实验。和baseline的差别，个人认为主要就是MTMS的表征融合层、训练策略以及CTCVR损失的差异。另外，对比实验没有采用MMoE等方法，作者说是因为MMoE等主要是为较少任务设计的，不太适合百度超多场景的情况。

![img](https://pic4.zhimg.com/80/v2-a375fb56288437557a746f1b7155843f_1440w.webp)

多场景的指标比单个场景的好，说明MTMS能够充分利用各个场景的数据，缓解数据稀疏性、减少负向迁移等影响。

## **Conclusion**

文章提出了一种比较简单的多目标多场景的建模范式，和主打共享的架构不同，

- 所有场景所有任务的embedding都是先并行、独立地训练到收敛，相当于多个子模型同时训练；
- 所有场景的表征一起融合用于scene & task-specific的DNN tower的微调和预测，这个步骤相当于将各个场景各个任务的信息做了融合，相辅相成；
- 这一架构拓展性很好，百度有数百个场景，有很强的适用性。

到此为止，我们可以对比下共享派和拆塔派在多任务多场景建模上的一些差异：

- 共享派代表作快手的PEPNet，通过个性化动态权重的方式来自适应地选择表征和DNN网络参数，达到缓解跷跷板现象的目标。这个工作的难点在于，个性化Gate网络能否真正学好怎么选择合适的embedding和DNN网络参数？如果一个用户行为比较稀疏，是否还能学好？当加入新的场景时，重新训练可能会影响已有场景的一些效果。
- 拆塔派代表作百度的MTMS，通过每个场景训练独立的表征向量，再通过融合层在finetune阶段对DNN进行微调，能够非常好地缓解跷跷板现象，对于场景的拓展性也很友好。难点在于尽管融合层finetune能够借鉴一些跨场景的信息，但embedding update过程中，数据稀疏的场景基础表征向量可能学习不充分；此外模型的容量很大、两阶段的训练范式的便捷性可能也需要在工程上做一些支持。

## **References**

[2] Weijie Zhao, Deping Xie, Ronglai Jia, Yulei Qian, Ruiquan Ding, Mingming Sun, and Ping Li. Distributed hierarchical gpu parameter server for massive scale deep learning ads systems. In Proceedings of the 3rd Conference on Machine Learning and Systems (MLSys), Austin, TX, 2020.

[3] Xin Wang, Peng Yang, Shaopeng Chen, Lin Liu, Lian Zhao, Jiacheng Guo, Mingming Sun, and Ping Li. Efﬁcient learning to learn a robust CTR model for web-scale online sponsored search advertising. In Proceedings of the 30th ACM International Conference on Information and Knowledge Management (CIKM), pages 4203–4213, Virtual Event, Australia, 2021.

[4] Miao Fan, Jiacheng Guo, Shuai Zhu, Shuo Miao, Mingming Sun, and Ping Li. MOBIUS: towards the next generation of query-ad matching in baidu’s sponsored search. In Proceedings of the 25th ACM SIGKDD International Conference on Knowledge Discovery & Data Mining (KDD), pages 2509–2517, Anchorage, AK, 2019.

[5] Zhiqiang Xu, Dong Li, Weijie Zhao, Xing Shen, Tianbo Huang, Xiaoyun Li, and Ping Li. Agile and accurate ctr prediction model training for massive-scale online advertising systems. In Proceedings of the International Conference on Management of Data (SIGMOD), pages 2404–2409, Virtual Event, China, 2021.

[6] Hongliang Fei, Jingyuan Zhang, Xingxuan Zhou, Junhao Zhao, Xinyang Qi, and Ping Li. Gemnn: Gating-enhanced multi-task neural networks with feature interaction learning for CTR prediction. In Proceedings of the 44th International ACM SIGIR Conference on Research and Development in Information Retrieval (SIGIR), pages 2166–2171, Virtual Event, Canada, 2021.

[7] Tan Yu, Xiaokang Li, Jianwen Xie, Ruiyang Yin, Qing Xu, and Ping Li. MixBERT for image-ad relevance scoring in advertising. In Proceedings of the 30th ACM International Conference on Information and Knowledge Management (CIKM), pages 3597–3602, Virtual Event, Australia, 2021.

[8] Tan Yu, Xuemeng Yang, Yan Jiang, Hongfang Zhang, Weijie Zhao, and Ping Li. TIRA in baidu image advertising. In Proceedings of the 37th IEEE International Conference on Data Engineering (ICDE), pages 2207–2212, Chania, Greece, 2021.

[9] Tan Yu, Yi Yang, Yi Li, Xiaodong Chen, Mingming Sun, and Ping Li. Combo-attention network for baidu video advertising. In Proceedings of the 26th ACM SIGKDD Conference on Knowledge Discovery and Data Mining (KDD), pages 2474–2482, Virtual Event, CA, 2020.







> 很明显一个缺点啊
>
> 1. 新场景重新训练（尤其严重，影响之前的使用）
> 2. 新特征重新训练









