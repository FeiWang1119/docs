# 单例模式(Singleton Pattern)

目的  
- 确保一个类只有一个实例，并提供该实例的全局访问点。
- Encapsulated "just-in-time initialization" or "initialization on first use".

单例模式的要点有二个：

- 某个类只能有一个实例；
- 自行创建这个实例, 并提供该实例的全局访问点。

在单例模式的实现过程中，需要注意如下三点：

- 构造函数为私有，禁止拷贝构造和赋值（= delete）；
- 一个自身的静态私有成员变量；
- 一个公有的静态工厂方法。

单例设计模式分为两种：

- 饿汉式：类加载时就会创建该单实例对象。
优点：线程安全。  
缺点：内存浪费。

- 懒汉式：单实例对象不会在类加载时被创建，而是在首次使用该对象时创建。
优点：按需创建，节省资源。  
缺点：相对复杂，多线程需要加双重检查锁机制确保线程安全。

实例：
打印池管理打印任务的应用程序，通过打印池用户可以删除、中止或者改变打印任务的优先级，在一个系统中只允许运行一个打印池对象，如果重复创建打印池则抛出异常。现使用单例模式来模拟实现打印池的设计

```cpp
class GlobalClass {
   int    m_value;
   static GlobalClass* s_instance;
   GlobalClass( int v=0 ) { m_value = v; }
public:
   int  get_value()        { return m_value; }
   void set_value( int v ) { m_value = v; }
   static GlobalClass* instance() {
      if ( ! s_instance )
         s_instance = new GlobalClass;
      return s_instance;
   }
};
```
```
