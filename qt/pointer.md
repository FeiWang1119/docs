# 智能指针

智能指针是一种抽象数据类型，它具有标准指针的所有特性，并额外提供自动垃圾收集功能。 智能指针有助于动态内存操作。 它们的主要优点是减少了由于内存管理不善而导致的内存泄漏和错误。

# qt 中的智能指针

- QSharedDataPointer
- QExplicitlySharedDataPointer
- QtPatternist::AutoPtr (internal class)
- QSharedPointer
- QWeakPointer
- QPointer since Qt5 a wrapper around QWeakPointer. The class was once planned to be deprecated but instead was kept to support legacy code.
- QGuard (internal class)
- QScopedPointer
- QScopedArrayPointer

按时间顺序：

- QPointer（4.0）
- QSharedDataPointer (4.0)
- QExplicitlySharedDataPointer (4.3/4.4)
- QtPatternist::AutoPtr（internal class，4.4）
- QSharedPointer (4.5)
- QWeakPointer (4.5)
- QGuard（internal class，4.6）
- QScopedPointer (4.6)

# 共享指针与共享数据

首先，共享指针和共享数据是有区别的。当共享指针时，指针的值及其生命周期受到智能指针类的保护。换句话说，指针是不变量。但是，指针指向的对象完全不受其控制。不知道该对象是否可复制，是否可分配。
现在，数据共享涉及到智能指针类对共享数据有所了解。事实上，关键在于数据正在共享，我们不在乎如何共享。在这一点上，使用指针来共享数据这一事实是无关紧要的。例如，您真的不关心 Qt 工具类是如何隐式共享的，对吗？对您来说重要的是它们是共享的（因此减少了内存消耗）并且它们的工作方式就好像它们没有共享一样。

# 强指针引用与弱指针引用

强引用和弱引用之间的区别在于给定指针上的智能指针类的存在是否保证该对象不会被删除。换句话说，如果你有这个智能指针，你确定它会一直有效吗（当然，前提是每个人都按照相同的规则玩）？
上面的一些指针类并不能保证这一点。如果他们不保证该对象仍然有效，那么他们的主要目的就是告诉您该对象是否已被删除。有些类可能会提供额外的功能，允许您将弱指针提升为强指针，从而保证它不会再被删除。

# Qt 智能指针类

## QPointer  

QPointer 是一个弱指针类，它共享指针值，而不是数据。当数据销毁后，该指针自动清零，防止成为野指针。它只对QObject和 QObject 派生类进行操作。这个类是在 Qt 4.0 中添加的，是 Qt 3 的QGuardedPtr（和 Qt 2 的QGuardedPtr）的直接升级。像它的前辈一样，QPointer 也受到破坏的常量支持的影响并显示出它的年龄。

它唯一目的是告诉你 QObject 是否已经被删除。但是，与 Qt 2 和 Qt 3 不同，Qt 4 的 QObject 可以存在于多个线程中。这意味着 QPointer 有一个严重的缺陷：它让你知道对象是否已被删除，但它不能保证下一行！例如，以下代码可能会遇到麻烦：

``` c++
 QPointer<QObject> o = getObject();
 // [...] 
 if (!o.isNull())
  o->setProperty( "objectName" , "Object" );
```
 
即使 isNull() 返回false，也不能保证该对象不会被下一行删除。

因此，如果您可以通过外部方式保证该对象不会被删除，则只能使用 QPointer 来访问该对象。例如，QWidget及其后代只能在 GUI 线程中创建、操作和删除。如果您的代码在 GUI 线程上运行或该线程被阻塞，那么 QPointer 的使用是安全的。

## QSharedDataPointer

现在这是一个不错的小班级。由于其独创性，它实际上是迄今为止 Qt 中最重要的智能指针类：它提供隐式共享和线程安全的写时复制。它要求您的类有一个名为ref的成员，它提供一个名为ref()的函数来增加引用计数，另一个名为deref() 的函数可以减少引用计数并在它降为零时返回false 。如果你从QSharedData派生你的类，你就会得到它。此外，QSharedDataPointer 对象的大小正好是指针的大小。这意味着您可以在代码中用它替换普通指针而不会破坏二进制兼容性。

此类是所有 Qt 值类型、隐式共享、线程安全的写时复制最新类的基础，例如QNetworkProxy。它没有在 QByteArray、QString 和 QList 等基类中使用的唯一原因是这些类是在创建此类之前开发的。从技术上讲，没有什么可以阻止使用 QSharedDataPointer 对这些类进行改造。

所以QSharedDataPointer是一个强大的智能指针类，共享数据。

##  QExplicitlySharedDataPointer
这个类与 QSharedDataPointer 完全一样（所以它是一个强大的智能指针类，共享数据），唯一的区别是它从不隐式地导致分离。使用 QSharedDataPointer，任何非常量访问都将导致数据被复制。对于 QExplicitlySharedDataPointer，您必须调用detach()才能实现。这允许您实现显式共享的数据类——Qt 不再有，但 Qt 3 在QMemArray中实现了（因此它存在于 Qt4 的 Qt3Support Q3MemArray中）。

但它也允许您对分离操作进行更细粒度的控制。事实上，如果 Qt 工具类要用智能指针类进行改造，它们会改用 QExplicitlySharedDataPointer。使用此类允许代码将分离延迟到最后一刻，确保不会发生不必要的内存访问。

## QtPatternist::AutoPtr
这是QtXmlPatterns模块使用的内部类。它基本上是你的股票，愚蠢的指针包装器。所以它实现了一个强 指针。但是，它不共享它。

此类首先存在的原因是 QtXmlPatterns 模块在内部广泛使用了异常。为了在不泄漏内存的情况下抛出异常，指示了一个指针包装器。QtXmlPatterns 还使用引用计数的类，其中未指定 AutoPtr——在这种情况下，它使用 QExplicitlySharedDataPointer。

5.QSharedPointer
此类是作为对 QtPatternist::AutoPtr 的响应而创建的。当我开始编写它时，我打算让它为 Qt 4.4 做好准备，并取代 Frans 编写的内部类的使用以及我认为是对 QExplicitlySharedDataPointer 的误用。QtXmlPatterns 使用 QExplicitlySharedDataPointer 不是为了共享数据，而是为了共享指针。它共享的对象不可复制。然而，后来的调查显示QtScript、Phonon和Solid出于相同目的使用它。（事实上​​，QtScript 在 4.3 中为此目的引入了 QExplicitlySharedDataPointer）

所以QSharedPointer在4.4就被搁置了，4.5又重生了。它实现了一个强大的智能指针类，共享指针。它具有您在现代指针类中可能需要的所有功能：它是多态的，它支持静态、常量和动态转换，它实现原子引用计数和线程安全语义，它支持自定义删除器。但请注意，当我说它实现线程安全语义时，它只是针对指针本身：记住它共享指针，而不是数据。

不过，它是有代价的：为了正确支持多态性，QSharedPointer 的大小实际上是普通指针大小的两倍。这意味着在 API 的公共部分用它替换普通指针时，您无法保持二进制兼容性。不过，您可以在代码内部使用它。

## QWeakPointer
这是 QSharedPointer 的伴随类。如果说实现了对指针的强控制，那么QWeakPointer就是一个弱智能指针类，共享指针。它与 QSharedPointer 协同工作：QWeakPointer 只能从 QSharedPointer 创建，并且当 QSharedPointer 被删除时它们会通知您。

不过，它们可以以线程安全的方式提升为 QSharedPointer。所以它允许我们重写上面的代码以更安全：

    QWeakPointer<Data> weak(getSharedPointer());
 // [...]

 QSharedPointer<Data> ptr = weak; 如果(!ptr.isNull()) ptr->doSomething();

 在这种情况下，将 QWeakPointer 提升为 QSharedPointer 要么成功，要么不成功。但这是一个线程安全的决定：如果它确实成功了，那么在您持有ptr引用时，保证不会删除生成的对象（同样，只要每个人都遵守相同的规则）。

在 4.6 中，我向 QWeakPointer 添加了一个漂亮的新功能：它也可以跟踪 QObject，而无需通过 QSharedPointer。它可用于确定 QObject 派生对象是否已被删除。所以它实现了一个弱指针类，共享QObject派生类的指针值。听起来很熟悉？是的，就是这个想法：您可以用更快、更现代的替代品替换旧的、慢速的 QPointer。请注意 QWeakPointer 的大小与 QPointer 的大小不同。

## QGuard
这是另一个内部类。添加它是为了替换 QPointer，因为它非常慢（它使用全局的、受互斥锁保护的 QHash，每个 QObject 析构函数都必须访问它）。这实际上是促使我编写 QWeakPointer QObject 跟踪功能的原因。但它处于不断变化的状态：我们不知道我们是否要保留甚至使用这个类。反正都是内部的，你真的不用管它。

## QScopedPointer
这是块中的新成员：它实现了一个非共享的强 指针包装器。它的创建是因为我们试图在我们的容器类中处理 Symbian 平台的异常：我们需要一种释放资源的方法，而无需在所有地方编写 try/catch。作用域指针提供了一种非常好的执行RAII 的方法。事实上，QScopedPointer 实际上是 QtXmlPattern 的 QtPatternist::AutoPtr 的完全替代。两者实现相同的功能，因此可以删除内部的。

有些人在 Harald 的博客中评论说我们可以使用 QSharedPointer。实际上，我们不能：QSharedPointer 有两个指针的大小，但我们要替换具有一个指针大小的 Qt 代码，所以我们需要一个适合该空间的类。这也是 QScopedPointer 将自定义删除器作为模板参数的原因，而不是构造函数的参数（就像 QSharedPointer 那样）：它在这 4 或 8 个字节中没有空间来存储自定义删除器。

更重要的是，QSharedPointer 实现了原子引用计数。不要介意它是原子的：对于 QScopedPointer 试图解决的情况，引用计数是绝对不必要的。

为什么不是 C++0x？为什么不是 TR1？为什么不升压？
Harald 博客中的一些人建议我们应该使用 std::shared_ptr (C++0x) 或 std::tr1::shared_ptr (TR1)。抱歉，那些人没有看得很远：我们不能使用 C++0x。它甚至没有被批准，只有两个编译器实现了对它的初始支持（GCC 4.3 和 MSVC 2010，处于测试阶段）。在这一点上建议对 Qt 使用 C++0x 甚至都不好笑。你可以将它用于你自己的代码，但我们不能在 Qt 中使用它。

TR1 已被更多的编译器实现。不幸的是，还不够。我们必须应对尚未完全实现 C++98 的编译器——或者那些懒得更改其编译器设置的人。例如，Solaris 上最新版本的 Sun Studio 编译器（Sun Studio 12，带有 CC 5.10）仍然带有 C++98 之前的 STL 的 RogueWave 实现。如果您阅读了Sun 将 RW stdlib 与 stlport4 进行比较的文章，您就会明白为什么他们仍然将这个已有 11 年历史的库保留为默认库。但关键是他们这样做了，这意味着我们必须处理它。（幸运的是，其他编译器供应商提供更新的 STL 实现，即使他们的编译器有时过于挑剔）

这意味着我们可以在 Qt 中使用的唯一来自 STL 的智能指针是 std::auto_ptr。即使这样也存在问题（RW stdlib 不实现成员模板）。

剩下的就是 Boost。而且Boost里面还有一些不错的东西：boost::shared_ptr, boost::intrusive_ptr, boost::scoped_ptr等等。其实Boost里面有很多不错的东西。我经常在那里看到我想在 Qt 中拥有的东西。当然，这意味着我也可以将上述功能添加到 Qt 中。没有什么能阻止我，除了我的日常工作 :-)

boost 的主要问题之一是它提供了一个“非 Qt-ish”的 API——至少可以这样说；我更喜欢称它为“可怕的 API”，但这是一种观点陈述，而不是事实。即使 Boost 的 API 对某些人来说是直观的，它也代表着与 Qt 的 API 的背离。这意味着那些使用 Qt 和 Boost 的人也需要学习 Boost 的做事方式，他们的函数命名等。

至少，我们必须用 Qt shell 包装 Boost 的 API。但如果我们更进一步，我们会发现 Qt 失去了对其一项重要技术的控制。然后，我们必须按照他们的时间表处理他们遇到的任何问题。此外，它还增加了对 Qt 的依赖性，我们无法证明这一点是合理的，因为它们不承诺二进制兼容性（通过网络粗略搜索；如果我错了请纠正我）。二进制兼容性是另一个主要问题。

所以，不，Boost 也不是一个选择。

结论
所以Qt的智能指针类太多了。或者是吗？

事实上，如果排除内部类并弃用 QPointer，Qt 只有这些指针类：

班级	描述
QSharedDataPointer / QExplicitlySharedDataPointer	分别隐式和显式实现数据共享（不是指针）
QSharedPointer	实现指针的引用计数强共享
Q弱指针	实现指针的引用计数弱共享
QScopedPointer / QScopedArrayPointer	实现非引用计数的强指针包装器（QSharedPointer 的小弟）
更新1：当目标类包含引用计数器时，QExplicitlySharedDataPointer可用于实现指针的引用计数共享（类似于boost::intrusive_ptr）

更新 2：QScopedPointer 确实基于 boost::scoped_ptr 的 API（但不是副本）；QSharedPointer 和 QWeakPointer 是从零开始开发的。