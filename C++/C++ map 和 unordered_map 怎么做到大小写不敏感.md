

```cpp
/*
 * @,@Author: ,: your name
 * @,@Date: ,: 2020-12-09 09:25:03
 * @,@LastEditTime: ,: 2020-12-09 10:15:22
 * @,@LastEditors: ,: Please set LastEditors
 * @,@Description: ,: In User Settings Edit
 * @,@FilePath: ,: /pluginManagerFramework/home/ok/test/f.cpp
 */
#include <iostream>
#include <map>
using namespace std;
/************************************************************************/
/* Comparator for case-insensitive comparison in STL assos. containers  */
/************************************************************************/
struct ci_less : std::binary_function<std::string, std::string, bool>
{
  // case-independent (ci) compare_less binary function
  struct nocase_compare : public std::binary_function<unsigned char, unsigned char, bool>
  {
    bool operator()(const unsigned char &c1, const unsigned char &c2) const
    {
      return tolower(c1) < tolower(c2);
    }
  };
  bool operator()(const std::string &s1, const std::string &s2) const
  {
    return std::lexicographical_compare(s1.begin(), s1.end(), // source range
                                        s2.begin(), s2.end(), // dest range
                                        nocase_compare());    // comparison
  }
};
 
int main()
{
  std::map<std::string, std::string, ci_less> aMap;
  map<string, string, ci_less>::iterator iter;
  std::map<std::string, std::string> bMap;
  
  bMap.insert(make_pair("hello", "w"));
  bMap.insert(make_pair("String", "orld"));
  bMap.insert(make_pair("HARRY", "world"));
  bMap.insert(make_pair("hI", "world123"));
  bMap.insert(make_pair("Abc", "world234"));
  bMap.insert(make_pair("EI", "world56"));
  bMap.insert(make_pair("At", "world78"));
  bMap.insert(make_pair("Bring123", "world90"));
  
  aMap.insert(bMap.begin(), bMap.end()); 
 
  iter = aMap.find("HELLO");
  if (iter != aMap.end())
  {
    cout << "Find, the value is " << iter->second << endl;
  }
  else
  {
    cout << "Do not Find" << endl;
  }
 
  iter = aMap.find("at");
  if (iter != aMap.end())
  {
    cout << "Find, the value is " << iter->second << endl;
  }
  else
  {
    cout << "Do not Find" << endl;
  }
 
  iter = aMap.find("BRING123");
  if (iter != aMap.end())
  {
    cout << "find! the value is : " << aMap["BRING123"] << endl;
  }
  else
  {
    cout << "not find!" << endl;
  }
 
  if (aMap.count("BRING123"))
  {
    cout << "find! the value is : " << aMap["BRING123"] << endl;
  }
  else
  {
    cout << " not Find" << endl;
  }
 
  if (aMap.count("BRING1"))
  {
    cout << "find! the value is : " << aMap["BRING1"] << endl;
  }
  else
  {
    cout << " not Find" << endl;
  }
 
  return 0;
}
```



​	普通的map只包含了key和value，在比较的时候是严格区分大小写的。C++的Map还提供了一个自定义比较器的定义方式，如果我们定义一个实现了不区分大小写的比较器放到map的声明里，那后面在查找key时就可以不区分大小写了，这里的查找包括find()和count()俩函数