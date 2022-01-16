# TF-Serving



### 作用

在使用TensorFlow 进行模型训练、验证和预测中都是基于训练的本身程序进行的实现，但模型完善之后的生产上线流程，就变得五花八门了。

为什么要用TF-Serving？

1. 举个例子，线上请求一张mnist 手写图片用于在线预测这个图片的代表的数字，而我们不可能将这张图片塞进去测试数据集当中，再 `train` 一遍模型，然后根据模型测试结果再返回结果。（1.耗时长 2.重复性计算 3.线上逻辑复杂）
2. 提供统一标准效仿（许多公司用于在线预测的开发模块不尽相同，TF-Serving 仅是其中一个实验，也可以通过TensorFlow 提供接口自己开发一个用于类似在线预测功能的模块。）

### 方式与功能

gRPC、Restful API

gRPC 基于两个相互的C/S 客户端



功能

- 版本控制（热更新）
- 支持多模型请求



### 多模型版本控制

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190116225421294.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0plcnJ5WmhhbmdfXw==,size_16,color_FFFFFF,t_70)

首先需要基于在线预测模块都是基于 `savedmodel` 模型保存文件进行开发。使用多模型多版本进行控制，默认情况下回加载具有较大版本号数字的模型。

#### 多模型的部署

在multiModel文件夹下新建一个配置文件`model.config`，文件内容为：

```sh
model_config_list:{
    config:{
      name:"model1",
      base_path:"/models/multiModel/model1",
      model_platform:"tensorflow"
    },
    config:{
      name:"model2",
      base_path:"/models/multiModel/model2",
      model_platform:"tensorflow"
    },
    config:{
      name:"model3",
      base_path:"/models/multiModel/model3",
      model_platform:"tensorflow"
    } 
}

```

配置文件定义了模型的名称和模型在容器内的路径，现在运行tfserving容器 :

```shell
docker run -p 8501:8501 --mount type=bind,source=/home/jerry/tmp/multiModel/,target=/models/multiModel \
 -t tensorflow/serving --model_config_file=/models/multiModel/models.config
```

**请求预测**：

```python
import requests 
import numpy as np 
SERVER_URL = 'http://localhost:8501/v1/models/model3:predict'  
#注意SERVER_URL中的‘model3’是config文件中定义的模型name,不是文件夹名称

def prediction(): 
    predict_request='{"instances":%s}' % str([[[10]*7]*7]) 
    print(predict_request) 
    response = requests.post(SERVER_URL, data=predict_request) 
    print(response)
    prediction = response.json()['predictions'][0] 
    print(prediction) 

if __name__ == "__main__": 
    prediction()
```



#### 指定模型版本

如果一个模型有多个版本，并在预测的时候希望指定模型的版本，可以通过以下方式实现。
修改[model](https://so.csdn.net/so/search?q=model&spm=1001.2101.3001.7020).config文件，增加model_version_policy：

```sh
model_config_list:{
    config:{
      name:"model1",
      base_path:"/models/multiModel/model1",
      model_platform:"tensorflow",
      model_version_policy:{
        all:{}
      }
    },
    config:{
      name:"model2",
      base_path:"/models/multiModel/model2",
      model_platform:"tensorflow"
    },
    config:{
      name:"model3",
      base_path:"/models/multiModel/model3",
      model_platform:"tensorflow"
    } 
}
```

请求预测的时候，如果要使用版本为100001的模型，只要修改SERVER_URL为：

```sh
SERVER_URL = 'http://localhost:8501/v1/models/model1/versions/100001:predict' 
```

tfserving支持模型的Hot Plug，上述容器运行起来之后，如果在宿主机的 /home/jerry/tmp/multiModel/model1/ 文件夹下新增模型文件如100003/，tfserving会自动加载新模型；同样如果移除现有模型，tfserving也会自动卸载模型。



借鉴

- https://blog.csdn.net/JerryZhang__/article/details/86516428



