# 神器 Nginx 的学习手册

转载自：https://mp.weixin.qq.com/s/6iRnMgc-IgRr8v2uh3s2UA

### **Nginx 知识网结构图**

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070f2NxYCRUkenCHGZCibFJicDiaCv9BcXgL4kRDibhCteU6zwOCEOvH5mO3Q/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

### **正向代理**

局域网中的电脑用户想要直接访问网络是不可行的，只能通过代理服务器来访问，这种代理服务就被称为正向代理。

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070PPlwbISQSBzhf1sAeeqNwUHQagLPtWwvHjA00mWluTYThibZnqnnia4g/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

### **反向代理**

客户端无法感知代理，因为客户端访问网络不需要配置，只要把请求发送到反向代理服务器，由反向代理服务器去选择目标服务器获取数据，然后再返回到客户端。



此时反向代理服务器和目标服务器对外就是一个服务器，暴露的是代理服务器地址，隐藏了真实服务器 IP 地址。



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070CW3mYFhBcrgyGPrAXajVNPYB77RxkwVMjjY6HzEMIGCRaQXkWnbMxg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

**03**

### **负载均衡**

客户端发送多个请求到服务器，服务器处理请求，有一些可能要与数据库进行交互，服务器处理完毕之后，再将结果返回给客户端。



普通请求和响应过程如下图：



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070ElynCNRq7Pwz1v2fjupnaIib9ury1RpHVfqicNeYsymMB1Lsh6yZLjHA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070mYLYl3fEeXcNmh01c5Ue9cf72y8JVHPujiabqzn3PRG0icicrKg5jvibiag/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



### **动静分离**

为了加快网站的解析速度，可以把动态页面和静态页面交给不同的服务器来解析，加快解析的速度，降低由单个服务器的压力。



动静分离之前的状态：



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070FwK0Z5yaibzTV33iccaV5HuMQaLr1oibDbXJyRnphUdrNePibbibNLLJORw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

动静分离之后：



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u0701NlibJ7Qibnr5AoewJdI8nMEhLO3tqEUqQV9X60HvqXcQh5g7yib2kYlg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



### **Nginx安装**

Nginx 如何在 Linux 安装



参考链接：

```
https://blog.csdn.net/yujing1314/article/details/97267369
```

Nginx 常用命令



查看版本：

```
./nginx -v
```



启动：

```
./nginx
```



关闭（有两种方式，推荐使用 ./nginx -s quit）：

```
 ./nginx -s stop
 ./nginx -s quit
```



重新加载 Nginx 配置：

```
./nginx -s reload
```



### 负载均衡方式

如下：

- 轮询（默认）。
- weight，代表权，权越高优先级越高。
- fair，按后端服务器的响应时间来分配请求，相应时间短的优先分配。
- ip_hash，每个请求按照访问 ip 的 hash 结果分配，这样每一个访客固定的访问一个后端服务器，可以解决 Session 的问题。

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u0701YP460onMRSUz1EViaG60zFc3eibkJVibsOpD4sI4e2W4AOQ5qeTSW9eg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1) 

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u0703OL89ibL8iag83gRicVLLicv4j7CVZE57FickVCHF6gdJvglic38fYP1G9AQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1) 

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070tLvpqiaUJST9yzwS78snQnANPBwkcNfmAYTict12ry4ibfjnvE8k1AtTw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)





什么是动静分离？把动态请求和静态请求分开，不是讲动态页面和静态页面物理分离，可以理解为 Nginx 处理静态页面，Tomcat 处理动态页面。

动静分离大致分为两种：

- 纯粹将静态文件独立成单独域名放在独立的服务器上，也是目前主流方案。
- 将动态跟静态文件混合在一起发布，通过 Nginx 分开。



动静分离图析：

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070LdXqsEJibWwcVz4zFdQamlpIp5FZlxh4AU3lxibTqSFyo2vr9VfkUJYg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

实战准备，准备静态文件：

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070bO5Fp4GaokbPWMUQVN5qVTEezeTnmy9t6qCibIOeOvfGQDj6HofJ2rA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070V5pH79DnRABQFtmCTaibBwoNcRd2iaMeNle8FqLNrn2xZ75I2uO5WEOg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

配置 Nginx，如下图：

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070iaVlGlNUe0fjsAxYU2v21rhaZZ0JuPOXfsVMibC2AESt5blIPkoRL1gA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

Nginx 高可用

如果 Nginx 出现问题：

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070r6rib3ltNLZicw5iceYtRvz7sgY8yZQmMXldgpG3Q3ZOWZ5o0rbkibib9qA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

解决办法：



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070AbQ0KH4Wu5y5pRwUnUbOXibR3UQoldXQ0lqMibXUhTvNGSQ6rTicIfxfA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

前期准备：

- **两台 Nginx 服务器**
- **安装 Keepalived**
- **虚拟 ip**



### 安装 Keepalived：

```
[root@192 usr]# yum install keepalived -y
[root@192 usr]# rpm -q -a keepalived
keepalived-1.3.5-16.el7.x86_64
```



修改配置文件：

```
[root@192 keepalived]# cd /etc/keepalived
[root@192 keepalived]# vi keepalived.conf
```



分别将如下配置文件复制粘贴，覆盖掉 keepalived.conf，虚拟 ip 为 192.168.25.50。



对应主机 ip 需要修改的是：

- smtp_server 192.168.25.147（主）smtp_server 192.168.25.147（备）
- state MASTER（主） state BACKUP（备）

```
global_defs {
   notification_email {
     acassen@firewall.loc
     failover@firewall.loc
     sysadmin@firewall.loc
   }
   notification_email_from Alexandre.Cassen@firewall.loc
   smtp_server 192.168.25.147
   smtp_connect_timeout 30
   router_id LVS_DEVEL # 访问的主机地址
}

vrrp_script chk_nginx {
  script "/usr/local/src/nginx_check.sh"  # 检测文件的地址
  interval 2   # 检测脚本执行的间隔
  weight 2   # 权重
}

vrrp_instance VI_1 {
    state BACKUP    # 主机MASTER、备机BACKUP    
    interface ens33   # 网卡
    virtual_router_id 51 # 同一组需一致
    priority 90  # 访问优先级，主机值较大，备机较小
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.25.50  # 虚拟ip
    }
}
```



启动代码如下：

```
[root@192 sbin]# systemctl start keepalived.service
```



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070rxeYx90zzrcUfHmnJwRelYdnJr1sXrBScUPSD8yIib6D4nQkxKdqweQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)



访问虚拟 ip 成功：

![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070JJMVSYVJnVhySneVp7C1Y7jMmcMoPwbVjZzGVe1iczcqJzpDrSRLVoA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

关闭主机 147 的 Nginx 和 Keepalived，发现仍然可以访问。



原理解析



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u0706jB3SfG0RrqdmaU8HE6e5A4qTynwactlhBMmM0wqNicGGws98kCdiaqQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

如下图，就是启动了一个 master，一个 worker，master 是管理员，worker是具体工作的进程。



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070muQU3rnx4KFwDOmx2ovwgFbmpu6o6ey0kZKWuWgjhSwDhohkpVNeCA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

worker 如何工作？如下图：



![图片](https://mmbiz.qpic.cn/mmbiz_png/MOwlO0INfQrRD4Mcr0icGmGAj3uB9u070giaYRsmv7NU97QWqHra8HENYBGZib3DyEnVg0Qbky5Xzk1D9Tu87DFqA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)































