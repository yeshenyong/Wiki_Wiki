std::any_of 是 C++11 中引入的 STL 算法

当您有一个元素范围并且想要检查范围中的任何给定元素是否满足给定条件时，此 STL 算法很有用

std::any_of() 接受元素范围和一元谓词（回调）作为参数。

```cpp
bool any_of ( InputIterator start, InputIterator end, UnaryPredicate callback ) ;
```

如何将 std::any_of() 与两种不同类型的回调（即 Lambda 函数和函数指针）一起使用

```cpp
/*
Check if vector contains any string with size 4.
*/
bool result = std::any_of(vecOfStrs.begin(), vecOfStrs.end(), [](const std::string & str){
                                                        return str.size() == 4;
                                                        });
```

现在让我们检查给定的字符串是否包含所有小写字母，即不是单个大写字符，即

```cpp
/*
 Check if given string contains all lower case letters i.e. not a single upper case char
 */
result = std::any_of(str.begin(), str.end(), ::isupper);
```

