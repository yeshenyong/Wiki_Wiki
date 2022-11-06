https://stackoverflow.com/questions/60528172/whats-the-meaning-of-stdenable-if-t-0

```cpp
template <typename Integer,
          std::enable_if_t<std::is_integral<Integer>::value, int> = 0 >
T(Integer) : m_type(int_t) {}
```

如果`Interger` 为 `Interger` 则编译后，实际为

```cpp
template <typename Integer,
          int = 0 >
T(Integer) : m_type(int_t) {}
```

赋予不命名常量为0 的意义是您将根本无法使用此构造函数，因为无法指定构造函数的模板参数。即使这不是构造函数，您仍然希望使用默认值，以便用户不需要将不需要的值传递给未命名和未使用的模板参数。

目的是为了模板参数的筛选逻辑

