# MySQL



表定义文件

例如创建一个名为MyTable 的表，MySQL会在MyTable.frm文件中保存该表定义



对于表信息的查询

```mysql
SHOW TABLE STATUS LIKE 'user'
```



新版本的SQL 语法更复杂，针对复杂查询增加了许多的特性和改进，这对于简单查询可能带来了更多的开销。

旧版本的代码简单，对于简单的查询反而更有利。



MySQL 增删改不刷盘，会先写缓冲区，这时候其他连接MySQL的工具不能对行数据进行增删改



