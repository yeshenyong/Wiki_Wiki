# Linux 找到指定字符串出现次数

- 法一

```shell
grep -o objstr filename | wc -l
```



- 法二（多个objstr）

```shell
grep -o 'objstr\|objstr1' filename | wc -l
```



- 法三（vim 下）

```shell
:%s/objstr//gn
```





