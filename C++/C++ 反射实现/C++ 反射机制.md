# 反射机制

本文旨在，用C++ 实现绝大部分反射功能，一步步完善

> 把问题列出来，就解决了一半了		—— 我

反射机制包含许多种类

1. 通过类名称字符串创建类的实例
2. 通过类名称字符串获取类中属性和方法（修改属性和方法的访问权限等）

而本身，C++ 不支持反射机制的实现。

反射机制的作用

​	反射技术以其明确分离描述系统自身结构、行为的信息与系统所处理的信息，建立可动态操纵的因果关联以动态调整系统行为的良好特征，已经从理论和技术研究走向实用化，使得动态获取和调整系统行为具备了坚实的基础。当需要编写扩展性较强的代码、处理在程序设计时并不确定的对象时，反射机制会展示其威力，这样的场合主要有：

1. 序列化（Serialization）和数据绑定（Data Binding）。
2. 远程方法调用（Remote Method Invocation RMI）。
3. 对象/关系数据映射（E/R Mapping）。

反射而反射机制又异常复杂

### 反射的种类与用法

举例

在Java 中的反射实现

```Java
public class Demo02 {
    @SuppressWarnings("all")
    public static void main(String[] args) throws Exception {
        // 另一个com.sg.myReflection.bean包下的User类
        String path = "com.sg.myReflection.bean.User";
        try {
            Class clazz = Class.forName(path);

            // 获取类名
            String strName01 = clazz.getName();// 获取完整类名com.sg.myReflection.bean.User
            String strName02 = clazz.getSimpleName();// 直接获取类名 User

            // 获取属性
            Field[] field01 = clazz.getFields(); // 返回属性为public的字段
            Field[] field02 = clazz.getDeclaredFields(); // 返回所有的属性
            Field field03 = clazz.getDeclaredField("id"); // 获取属性为id的字段

            // 获取普通方法
            Method[] Method01 = clazz.getDeclaredMethods(); // 返回public方法
            Method method = clazz.getDeclaredMethod("getId", null); // 返回getId这个方法，如果没有参数，就默认为null


            // 获取构造方法
            User u1 = (User) clazz.newInstance(); // 获取无参的构造函数这里的前提的保证类中应该有无参的构造函数
            // 获取参数为(int,String,int)的构造函数
            Constructor c2 = clazz.getDeclaredConstructor(int.class, String.class, int.class);
            // 通过有参构造函数创建对象
            User u2 = (User) c2.newInstance(1001, "小小", 18);


            // 通过反射调用普通方法
            User u3 = (User) clazz.newInstance();
            Method method03 = clazz.getDeclaredMethod("setId", int.class);
            method.invoke(u3, 1002); // 把对象u3的id设置为1002



            // 通过反射操作普通的属性
            User u4 = (User) clazz.newInstance();
            Field f = clazz.getDeclaredField("name");
            f.setAccessible(true); // 设置属性可以直接的进行访问
            f.set(u4, "石头");

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}
```



### 

### 通过类名称字符串创建类的实例

C++ 实现此功能