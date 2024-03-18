# RAGxporler 技术梳理

https://github.com/gabrielchua/RAGxplorer

```python
# pip install ragxplorer

from ragxplorer import RAGxplorer
client = RAGxplorer(embedding_model="thenlper/gte-large")
client.load_pdf("presentation.pdf", verbose=True)
client.visualize_query("What are the top revenue drivers for Microsoft?")
```

![img](https://raw.githubusercontent.com/gabrielchua/RAGxplorer/main/images/example.png)