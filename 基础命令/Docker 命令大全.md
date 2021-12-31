# Docker 命令大全



#### docker login



```
docker login 对应仓库
```



#### 拉取镜像

```
docker pull tensorflow/tensorflow:latest
```



#### 删除镜像

```shell
docker rmi image_name:tag
```



#### 镜像生成容器

```bash
docker run --net=host  -d -t --name tensorflow  tensorflow/tensorflow:1.8
```



#### 进入容器

```
docker exec -it containerID /bin/bash
```

**-i** 表示允许我们对容器内的 (STDIN) 进行交互

**-t** 表示在新容器内指定一个伪终端或终端

 /bin/bash ,这将在容器内启动 bash shell



#### 容器打包成镜像

```
sudo docker commit containerID imagesName:version
sudo docker push imagesName:version
```



#### 查看启动成功的docker

```
docker ps
```

可以查看到的才是启动成功，无法执行命令后发现没有 加 -a 参数 -a = all

```
docker logs [id]
```

#### 查看启动失败原因



#### 容器与主机文件传输

```
sudo docker cp host_path containerID:container_path
sudo docker cp containerID:container_path host_path
```



#### 删除容器

```
docker rm e11efb30362a 删除该容器.
```



#### 杀死实例

```
docker kill 实例名称或者实例ID 杀死正在运行的实例
```



#### 避免每次sudo

```
sudo usermod -aG docker $USER 把当前用户添加到docker组
newgrp - docker 刷新docker组
sudo service docker restart 重启服务
```



#### 查看仓库内镜像

```
sudo docker search centos    # 查看centos镜像是否存在
```



#### 交互模式进入容器

```
sudo docker exec -it 80e7f1bd27e0 /bin/bash
```



```
TESTDATA="$(pwd)/serving/tensorflow_serving/servables/tensorflow/testdata"
```



```
docker run --net=host  -d -t --name tensorflow  tensorflow/tensorflow:1.8
```



```python
$ sudo docker run -t --rm -p 8501:8501 \
    -v "$TESTDATA/saved_model_half_plus_two_cpu:/models/half_plus_two" \
    -e MODEL_NAME=half_plus_two \
    tensorflow/serving:1.12.0 &
```

```python
$  sudo docker run -p 8501:8501 --mount type=bind,\
  source=/home/yeshenyong.ysy/program/serving/tensorflow_serving/servables/tensorflow/testdata/saved_model_half_plus_two_cpu,\
target=/models/half_plus_two  -e MODEL_NAME=half_plus_two -t tensorflow/serving &
```

```
--mount：   表示要进行挂载
-p:         指定主机到docker容器的端口映射.gRPC默认端口是8500，HTTP请求的默认端口是8501，
source：    指定要运行部署的模型地址， 也就是挂载的源。这里制定的是下载的Demo的模型存储的地址，如果是自己训练的模型，目录指定此模型所在位置
target:     挂载的目标位置，将模型加载到哪里，也就是挂载到docker容器中的哪个位置，这是docker容器中的目录。/models/是docker中默认模型位置, half_plus_two 是模型名，serving镜像中的程序会自动加载镜像内/models下的模型，通过MODEL_NAME指定/models下的哪个模型。
-t:  指定的是挂载到哪个容器
-e:  指定挂在的模型名称
```

```bsh
docker run -t --rm -p 8501:8501 \
    -v "/home/yeshenyong.ysy/program/serving-master/tensorflow_serving/servables/tensorflow/testdata/saved_model_half_plus_two_cpu:/models/half_plus_two" \
    -e MODEL_NAME=half_plus_two \
    tensorflow/serving:1.12.0 &
```

```
docker run -t --rm -p 8502:8501 \
    -v "/apsarapangu/disk5/modelName:/models/modelName" \
    -e MODEL_NAME=modelName \
    tensorflow/serving:1.12.0 &
```

```
docker run -t --rm -p 8502:8501 \
    -v "/apsarapangu/disk5/yeshenyong_job_dir/modelName:/models/modelName" \
    -e MODEL_NAME=modelName \
    tensorflow/serving:1.12.0 &

tensorflow_serving_model 

docker run -p 8501:8501 \
  --mount type=bind,source=/apsarapangu/disk5/modelName/,target=/models/modelName \
  -e MODEL_NAME=modelName -t tensorflow/serving:1.12.0
```

```
docker run --rm
--rm:这样在容器退出时就能够自动清理容器内部的文件系统

```

docker run --name 容器名 -it --mount source=卷名,target=容器内绝对路径(挂载点) 镜像名 bash或者
docker run --name 容器名 -it -v 卷名:容器内绝对路径(挂载点) 镜像名 bash
挂载就是用–mount参数 source是数据卷,target是容器内文件系统挂载点,







## dockerfile 详解



​	Dockerfile的语法非常简单，然而如何加快镜像构建速度，如何减少Docker镜像的大小却不是那么直观，需要积累实践经验。这篇博客可以帮助你快速掌握编写Dockerfile的技巧



​	dockerfile理解为一种配置文件，用来告诉docker build命令应该执行哪些操作。一个简易的Dockerfile文件如下所示，官方说明：[Dockerfile reference](https://link.zhihu.com/?target=https%3A//docs.docker.com/engine/reference/builder/)：

**目标:**

- 更快的构建速度
- 更小的Docker镜像大小
- 更少的Docker镜像层
- 充分利用镜像缓存
- 增加Dockerfile可读性
- 让Docker容器使用起来更简单



**总结**

- 编写.dockerignore文件
- 容器只运行单个应用
- 将多个RUN指令合并为一个
- 基础镜像的标签不要用latest
- 每个RUN指令后删除多余文件
- 选择合适的基础镜像(alpine版本最好)
- 设置WORKDIR和CMD
- 使用ENTRYPOINT (可选)
- 在entrypoint脚本中使用exec
- COPY与ADD优先使用前者
- 合理调整COPY与RUN的顺序
- 设置默认的环境变量，映射端口和数据卷
- 使用LABEL设置镜像元数据
- 添加HEALTHCHECK





**示例**



**1. 编写.dockerignore文件**

构建镜像时，Docker需要先准备`context` ，将所有需要的文件收集到进程中。默认的`context`包含Dockerfile目录中的所有文件，但是实际上，**我们并不需要.git目录，node_modules目录等内容**。 `.dockerignore` 的作用和语法类似于 `.gitignore`，可以忽略一些不需要的文件，这样可以有效加快镜像构建时间，同时减少Docker镜像的大小。示例如下:

```
.git/
node_modules/
```



















osscmd

```shell
osscmd get oss://64931/loyoen/zeus_predict/zeus.tar ./zeus.tar --host=oss-cn-hangzhou-zmf.aliyuncs.com --id=AkAIKVfFNztU3ViB --key=XLoVyzfxpiIYhAhBxPZRdjvu1IUXUV
```















