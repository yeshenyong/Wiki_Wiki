例子

Air-raps-server 中使用到的`crypto` 密码库

1. 查看README.md、install.txt 文档（全英的着实有点吐血）
2. makefile 文件 make、make test、make install
3. stackoverflow 上直接搞安装步骤（最方便、但如果以后工作中用到公司内部就不友好了，多学学看还是不亏的）



总结

基本就是编译一遍，将头文件、库文件cp 到/usr/bin、/usr/include 文件夹下



然后gcc 编译加 -lpthread -lcryptopp 类似