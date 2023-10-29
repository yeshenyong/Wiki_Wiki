[LLM RAG界最强检索方式: L1 Hybrid Retrieval+L2 Semantic Ranking (qq.com)](https://mp.weixin.qq.com/s/yKknJQi8Yu0XlquBuufA2Q)



**Retrieval （检索）** --通常称为 L1，这一步骤的目标是从索引中快速找到满足搜索条件的所有文档--可能是数百万或数十亿的文档。对这些文档进行评分，选出前几名（通常在 50 名左右）返回给用户或提供给下一层。Azure 认知搜索支持 3 种不同的 L1 模式：

- Keyword：使用传统的全文搜索方法--通过特定语言的文本分析将内容分解为术语，创建反向索引以实现快速检索，并使用 BM25 概率模型进行评分。
- Vector：使用嵌入模型将文档从文本转换为矢量表示。检索是通过生成查询嵌入并找到其向量与查询向量最接近的文档来进行的。我们使用 Azure Open AI text-embedding-ada-002 (Ada-002) 嵌入和余弦相似性进行本帖中的所有测试。
- Hybrid：同时执行关键词和向量检索，并应用融合步骤从每种技术中选择最佳结果。Azure 认知搜索目前使用Reciprocal Rank Fusion （RRF）来生成单一结果集。



**Ranking** --也称为 L2—使用top  L1 结果的子集，并计算更高质量的相关性分数来重新排列结果集。L2 可以提高 L1 的排名，因为它对每个结果应用了更强的计算能力。L2 排序器只能对 L1 已经找到的结果重新排序，如果 L1 错过了一个理想的文档，L2 就不能修复它。

Semantic ranking由 Azure 认知搜索的 L2 ranker执行，该排序器利用了从微软必应改编而来的多语言深度学习模型。语义排名器可对 L1 结果中的前 50 个结果进行排名。

截止到目前， 针对生成式AI ，**Hybrid Retrieval + Semantic Ranking**搜索的准确性是最高的。