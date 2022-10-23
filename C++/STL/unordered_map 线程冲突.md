# unordered_map 线程冲突

可能存在线程冲突的代码（伪代码）

```cpp
ReadLock(mutex_);
if (mymap["test"] == 0) { // map default value is 0
	// do else sth
}
```

若`mymap` 中的key 不存在 "test" 字符串，则会返回默认值0，且会触发 `mymap` 的写，写入默认值 0（会有读写冲突，因为内存可能移位）



好的解法

```cpp
ReadLock(mutex_);
if (mymap.find("test") == mymap.end()) {
	// do else sth
}
```





















