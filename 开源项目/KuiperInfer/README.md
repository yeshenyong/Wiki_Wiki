# KuiperInfer



开源项目地址：https://github.com/zjhellofss/KuiperInfer

辅助网站

https://netron.app/

数学库Openblas: https://github.com/xianyi/OpenBLAS

数学库Armadillo: https://arma.sourceforge.net/docs.html （只是个抽象接口层，底层盗用Openblas）

Caffe框架: https://github.com/BVLC/caffe





### 目录

source是源码目录

1. **data/** 是张量类Tensor的实现和Tensor初始化方法
2. **layer/** 是算子的实现
3. **parser/** 是Pnnx表达式的解析类
4. **runtime/** 是计算图结构，解析和运行时相关

**test**是单元测试目录，基本做到public方法单元测试权覆盖

**bench**是google benchmark, 包含对MobilenetV3, Resnet18和yolov5s的性能测试。











