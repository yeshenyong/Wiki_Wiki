# Considering Correctness（考虑正确性）

## Avoid Typeless Interfaces（避免无类型的接口）

Bad Idea:

```cpp
std::string find_file(const std::string &base, const std::string &pattern);
```

Better Idea:

```cpp
std::filesystem::path find_file(const std::filesystem::path &base, const std::regex &pattern);
```

上面的方法更好，但仍然存在从`std::string`到`std::filesystem::path`和返回的隐式转换

考虑使用类型安全的库，比如

- https://foonathan.net/type_safe/
- https://github.com/rollbear/strong_type
- https://github.com/joboccara/NamedType

注意，更强的类型还可以支持更多的编译器优化

- [Sorting in C vs C++](Sorting in C vs C++.pdf)