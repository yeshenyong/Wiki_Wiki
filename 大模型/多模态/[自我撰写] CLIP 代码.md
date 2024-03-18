# CLIP 代码阅读

CLIP（Contrastive Language-Image Pre-Training）OpenAI 在 2021 年初发布的用于匹配图像和文本的预训练神经网络模型。CLIP目前仍然是图文对齐模型的最优模型之一，它结构简单，由一个ResNet组成的图像表征编码器和一个基于bert结构的文本编码器组成（图像编码器和文本编码器可替换为不同版本，也有ViT版本的），在训练的时候，通过其提出的ITC Loss损失函数，输入图文对儿同时训练两个编码器，进行对比学习，最终将两个编码器的输出映射在一个向量空间。
https://arxiv.org/pdf/2103.00020.pdf

1.  训练侧：构建 text-image pair 对，batch 内进行对比学习 contrastive loss. 通过类似双塔结构将text-embedding & image-embedding 投影至相同空间里（其中Text-encoder/Image-encoder 也类似双塔可以进行不同模型的切换）
2.  推理侧：根据下游任务，输入sample = ["狗", "猫", "人", "老虎", "狮子"]，prompt 模板为"A photo of a {label}." 但也可以考虑加一些context比如"A photo of a {label}, a type of pet." 最后输出 text-encoder * image_encoder 相似最高的（内积），最后进行prompt template output（当然也可以加一些few shot）

```python
def encode_image(self, image):
    return self.visual(image.type(self.dtype))

def encode_text(self, text):
    x = self.token_embedding(text).type(self.dtype)  # [batch_size, n_ctx, d_model]
    x = x + self.positional_embedding.type(self.dtype)
    x = x.permute(1, 0, 2)  # NLD -> LND
    x = self.transformer(x)
    x = x.permute(1, 0, 2)  # LND -> NLD
    x = self.ln_final(x).type(self.dtype)

    # x.shape = [batch_size, n_ctx, transformer.width]
    # take features from the eot embedding (eot_token is the highest number in each sequence)
    x = x[torch.arange(x.shape[0]), text.argmax(dim=-1)] @ self.text_projection

    return x
```



```python
def forward(self, image, text):
    image_features = self.encode_image(image)
    text_features = self.encode_text(text)
    # normalized features
    image_features = image_features / image_features.norm(dim=1, keepdim=True)
    text_features = text_features / text_features.norm(dim=1, keepdim=True)

    # cosine similarity as logits
    logit_scale = self.logit_scale.exp()
    logits_per_image = logit_scale * image_features @ text_features.t()
    logits_per_text = logits_per_image.t()

    # shape = [global_batch_size, global_batch_size]
    return logits_per_image, logits_per_text
```

结论

- Zero-shot，对于下游任务来说，可以不需要进行额外的finetune 即可很好的适应其任务，并且text-image 的形式，对单纯image 建模分类任务（ResNet）的泛化性更强（一些没出现在数据集，但是与物体本身高度相似）domain adaptation（DA）/generalization（DG）中 DG 任务增强
- 本质上还是一个分类模型，不能像image-caption 一样，生成词语/生成图片