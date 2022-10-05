# Linux /etc 下汇总





### hosts

/etc/hosts 等关于host

hosts文件是linux系统中负责ip地址与域名快速解析的文件，以ASCII格式保存在/etc目录下，文件名为hosts，不同的linux版本，文件也可能不同，比如Debian的对应文件是/etc/hostname。

hosts文件包含了ip地址和主机名之间的映射，包括主机名的别名，在没有域名服务器的情况下，系统上的所有网络程序都通过查询该文件来解析对应于某个主机名的ip地址，否则就需要使用DNS服务程序来解决。通常可以将常用的域名和ip地址映射加入到hosts文件中，实现快速方便的访问 

优先级：**dns缓存>hosts>dns服务** 

hosts：the static table lookup for host name（主机名查询静态表）

### hosts.deny

控制禁止访问本机的IP，如果两个文件（allow 和 deny）的配置有冲突，以/etc/hosts.deny为准

### hosts.allow

控制可以访问本机的IP地址

### hostname

主机名





















