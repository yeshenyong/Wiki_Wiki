# Bert

训练object：NSP（next sentence predict）判断句子相似性[cls]+ MLM（Masked Language Model）
1）80%的时候是[MASK]。如，my dog is hairy——>my dog is [MASK]
2）10%的时候是随机的其他token。如，my dog is hairy——>my dog is apple
3）10%的时候是原来的token（保持不变，个人认为是作为2）所对应的负类）。如，my dog is hairy——>my dog is hairy
再用该位置对应的概率P 去预测出原来的token（输入到全连接，然后用softmax输出每个token的概率，最后用交叉熵计算loss）