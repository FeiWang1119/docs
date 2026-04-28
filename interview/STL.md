# C++ STL

## STL容器

### C++ STL 中主要有三类容器：

- 顺序容器（Sequence Containers）：如 vector, list, deque, array, forward_list
- 关联容器（Associative Containers）：如 set, map, multiset, multimap
- 无序容器（Unordered Containers）哈希容器：如 unordered_map, unordered_set, unordered_multimap, unordered_multiset

此外，STL 还包括配套的迭代器、算法、仿函数、适配器等组件。

- 顺序容器：维护元素的线性序列

vector：动态数组，支持快速随机访问;
deque：双端队列，两端高效插入/删除;
list：双向链表，任意位置高效插入/删除;
forward_list：单向链表，更节省空间;
array：固定大小数组，更安全的原生数组替代;
   
- 关联容器：基于键值对的有序存储

set/multiset：有序唯一/非唯一键集合
map/multimap：有序键值对集合，键唯一/非唯一

- 无序关联容器：基于哈希表的存储

unordered_set/unordered_multiset
unordered_map/unordered_multimap

每种容器在时间复杂度、内存布局和适用场景上有所不同。

### STL的容器的特点如下

- 顺序容器

|容器|特点|底层结构|
|---|---|---|
|vector|动态数组，随机访问快，尾部插入快，头部，中间插入需移动元素|动态数组|
|deque|双端队列，头尾都能快速插入/删除|动态数组|
|array|固定大小数组（C++11）|静态数组|
|list|双向链表，任意位置插入/删除快，但不支持随机访问|链表|
|forward_list|单向链表（C++11）|链表|

- 关联容器（有序，基于红黑树）

map/set 查找、插入都是 O(logN)，但有序

|容器|特点|底层结构|
|---|---|---|
|set|唯一元素自动排序|红黑树|
|multiset|元素允许重复|红黑树|
|map|键值对，键唯一|红黑树|
|multimap|键值对，键可重复|红黑树|

- 无序关联容器（基于哈希表，C++11）

查找、插入是 O(1) 平均复杂度

|容器|特点|底层结构|
|---|---|---|
|unordered_set|无序唯一集合|哈希表|
|unordered_multiset|无序可重复集合|哈希表|
|unordered_map|无序键值对|哈希表|
|unordered_multimap|无序可重复键值对|哈希表|

### 使用场景

vector：需要随机访问、动态扩容的情况（如图像处理、线性缓存）
list：频繁插入删除数据的场景（如编辑器撤回栈）
map/unordered_map：查找、计数、字典、配置解析（如记录访问次数）
set：需要去重、唯一性判断（如用户 ID 集合）
deque：实现滑动窗口算法、双端队列缓存（如 LRU）

## vector和array的使用场景分别是什么？ （考点：vector与array的使用场景）【简单】

|特性|std::vector|std::array）|
|---|---|---|
|大小|动态可变|固定不变（编译时确定）|
|内存分配|堆（动态分配）|栈（静态分配）|
|标准库支持|C++98|C++11|
|元素访问|支持下标访问和迭代器|同样支持|
|效率|稍慢（涉及动态分配）|更快（在栈上分配，无需额外开销）|

std::array对比传统数组的优势：
支持 STL 算法、迭代器
类型安全，避免退化为指针


## 描述一下vector是如何保证元素连续存储的吗？ （考点：vector的内存管理）【简单】

vector内部使用动态数组作为底层存储结构，内部使用了一个单一的内存块来存储所有的元素，并且管理这个内存块的大小和容量。
当vector的内存容量不够时，会重新分配一块更大的内存，再把旧的数组中的数据复制到新的内存块中。

## 当vector的空间不足以容纳更多元素时，它是如何扩容的？ （考点：vector的动态扩容机制）【中等】

vector在空间不足时，会进行扩容。 vector内部实现了一个内存分配函数 内存不够时会申请一块原内存1.5-2倍大小的新块，再把旧的数组复制到新的内存中。
vector扩容时，是使用移动语义复制对象还是直接拷贝对象，取决于对象是否支持移动语义，实现了移动构造函数。

## vector的push_back和emplace_back有什么区别？ （考点：vector的元素添加方式）【简单】

【简要回答】
std::vector的push_back和emplace_back都用于在向量的末尾添加元素，但它们添加元素的方式不同：
push_back：创建一个元素的副本或移动该元素，然后将其添加到向量的末尾。
emplace_back：在向量的末尾就地构造元素，避免了额外的复制或移动。

【详细回答】
push_back：
push_back方法接受一个参数，该参数可以是一个值、一个已存在的对象的引用，或者是要添加到向量末尾的元素的右值。
如果参数是一个右值，push_back将使用移动构造函数（如果可用）来移动它；如果参数是一个左值，它将使用复制构造函数来复制它。
push_back在向量需要扩容时，可能会导致向量中的所有元素被复制或移动到新的内存位置。
emplace_back：
emplace_back不接受任何直接的参数，但它接受构造新元素所需的参数，并使用这些参数列表在向量的末尾就地构造一个新元素。
emplace_back避免了临时对象的创建和销毁，因为它直接在向量内存空间内构造对象。
emplace_back不会触发向量内存的重新分配，除非向量已经达到了它的容量上限。

【知识拓展】
性能：emplace_back通常比push_back更高效，因为它省去了临时对象的创建和销毁的开销，特别是在构造和析构成本较高时。
适用场景：当你需要传递多个参数来构造向量末尾的元素时，emplace_back是一个很好的选择，因为它允许你直接传递这些参数给构造函数。
完美转发：emplace_back能够完美转发参数，这意味着参数的值类别（左值、右值）会被保留，这对于需要区分处理左值和右值的构造函数特别有用。


## list和vector有什么区别？ （考点：list与vector的比较）【中等】

- 底层数据结构：

vector：动态数组，在内存中是连续存储的。
list：双向链表，每个元素存储在不连续的内存块中，每个节点包含指向前一个元素和后一个元素的指针。

- 随机访问：

vector：支持高效的随机访问（O(1)），因为通过索引计算地址是常量时间。
list：不支持随机访问，访问第n个元素需要遍历链表（O(n)）。

- 插入和删除操作：

vector：
在尾部插入或删除：平均O(1)
在中间或头部插入或删除：O(n)，因为需要移动元素。
list：
在任意位置插入或删除：操作是O(1)，因为只需要调整指针。
但是，找到插入位置可能需要O(n)（除非已经拥有该位置的迭代器）。

- 迭代器：

vector：随机访问迭代器，支持所有迭代器操作（包括加减整数等）。
list：双向迭代器，只支持前移和后移（++和--），不支持随机访问（如it+5）。

- 使用场景：

vector：当需要频繁随机访问，或者主要在尾部添加/删除元素时使用。
list：当需要在容器中间频繁插入和删除，并且不需要随机访问时使用。

- 速度：
  
vector：使用连续内存分配，可以利用缓存的优势，提高访问速度。
这主要基于计算机系统中缓存的工作原理和局部性原理，局部性原理是如果某个数据被访问，那么不久之后它很可能再次被访问，它相邻的数据也很可能很快被访问。
std::vector的连续内存布局天然符合空间局部性。当程序访问一个元素时，相邻元素有很大概率也会被访问（例如遍历数组），因此缓存会一次性加载一整个缓存行到高速缓存中，
后续访问同一缓存行内的数据时，直接命中高速缓存，速度非常快。
list：节点分散在堆内存各处，访问下一个元素需跳转指针（而且可能跨越不同内存页），每次访问几乎都会导致缓存未命中，要重新访问主存，速度就慢。

## list是如何实现元素的插入和删除的？ （考点：list的元素操作）【中等】

list底层是双链表，使用指针链接元素的下一个和上一个元素，支持双向遍历。
插入和删除元素使用的就是双链表的插入和删除，修改指针索引。

## set的底层实现与map有什么不同？ （考点：set的底层数据结构）【中等】

std::set和std::map在C++标准库中都是基于平衡二叉树（通常是红黑树）实现的。主要区别在于：
std::set仅存储键值，而std::map存储键值对。
std::set的键值即为元素本身，而std::map的键值是键，值是与键相关联的数据。

## map、set、multimap、multiset有什么区别？ （考点：关联容器的比较）【中等】

C++中的std::map、std::set、std::multimap和std::multiset都是标准库中的关联容器，它们都基于红黑树实现。
以下是这些容器的详细区别：

- std::map

键值对存储：std::map存储键值对（std::pair<const Key, T>），其中Key是键，T是与键相关联的值。
键的唯一性：每个键在map中是唯一的，不允许有重复的键。
排序：map中的元素根据键自动排序。
访问：通过键访问值，使用下标操作符operator[]可以直接访问或添加元素。

- std::set

仅存储键：std::set只存储键（元素），不存储值。
元素唯一性：每个元素在set中是唯一的，不允许有重复的元素。
排序：set中的元素根据元素值自动排序。
访问：只能通过迭代器访问元素，不支持下标操作符。

- std::multimap

键值对存储：std::multimap存储键值对，与map类似，但是允许有重复的键。
键的重复性：允许多个元素具有相同的键。
排序：multimap中的元素根据键自动排序，对于具有相同键的元素，它们在内部是有序的（通常是插入顺序）。
访问：通过键访问值，但不能直接访问特定键的所有值，需要遍历。

- std::multiset
  
仅存储键：std::multiset只存储键（元素），与set类似，但是允许有重复的元素。
元素的重复性：允许多个元素具有相同的值。
排序：multiset中的元素根据元素值自动排序，对于具有相同值的元素，它们在内部是有序的（通常是插入顺序）。
访问：只能通过迭代器访问元素，不支持下标操作符。

## 如何在map和set中查找元素？ （考点：元素查找）【简单】

【简要回答】

find 方法：直接查找特定键的值，返回指向该元素的迭代器，如果未找到则返回end()迭代器。
count 方法：返回具find 方法，由于键的唯一性，返回值要么是0（未找到），要么是1（找到）。
lower_bound 方法：返回指向不小于给定键的首个元素的迭代器。
upper_bound 方法：返回指向大于给定键的首个元素的迭代器。
equal_range 方法：返回一个迭代器对，包括lower_bound和upper_bound，对于不存在的键，这两个迭代器是相同的。对于唯一键，不会返回相同值。

【详细回答】

find 方法：
直接使用 find 方法查找特定键的值。
如果找到了，返回一个指向该元素的迭代器；如果没有找到，返回一个指向 end() 的迭代器。
count 方法：
count 方法用于返回具有特定键的元素数量。由于 std::map 中的键是唯一的，所以返回值要么是0（未找到），要么是1（找到）。
lower_bound 和 upper_bound 方法：
lower_bound 返回指向不小于给定键的首个元素的迭代器。
upper_bound 返回指向大于给定键的首个元素的迭代器。
equal_range 方法：
equal_range 返回一个迭代器对，第一个是 lower_bound，第二个是 upper_bound。

## 容器 map 、deque、list的实现原理？（考点：map 、deque、list的底层数据结构）【简单】

- std::map:
基于红黑树：std:::map是基于红黑树实现的，它保证元素按键的顺序进行排序。
每个节点存储了键值对，并根据键的比较结果进行自平衡。红黑树是一种自平衡二叉搜索树，它通过颜色和节点的特定排列保持平衡。
有序容器：元素按照键的顺序自动排序，通常是按照小于运算符定义的顺序。
唯一键：每个键是唯一的，不允许有重复的键。
时间复杂度：提供对数时间复杂度(O(logn))的查找、插入和删除操作。
迭代器：由于map是基于树的，迭代器在遍历时是有序的。

- std::deque:
std::deque:是一个双端队列，允许在容器的两端进行快速的插入和删除操作。
它通常是基于一个节点数组实现的，每个节点存储一部分元素并可以动态的增加或删除节点。
允许序列操作：可以快速地在队列的前端和后端添加或删除元素。
时间复杂度：提供常数时间复杂度的前端和后端插入和删除操作。中间插入和删除操作可能需要O(n)。

- std::list:
std::list:是一个双向链表，提供了在任意位置快速插入和删除元素的能力。
每个节点包含数据和两个指针，分别指向前一个结点和后一个节点。
无序容器：元素在容器中没有特定的顺序。
插入和删除：提供高效的插入和删除操作，特别是当需要在容器中间插入和删除时。
时间复杂度：提供线性时间复杂度O(n)的查找操作，但插入和删除可以在O(1)时间内完成，前提是已经拥有指向待插入或删除元素的迭代器。
迭代器：由于std::list是线性结构，迭代器在遍历时是顺序的，但不支持随机访问。

## map && unordered_map的区别和实现机制 （考点：map 和 unordered_map的区别）【简单】

map底层是基于红⿊树实现的，因此map内部元素排列是有序的。
⽽unordered_map底层则是基于哈希表实现的，因此其元素的排列顺序是杂乱⽆序的。

## 迭代器在STL中扮演什么角色？ （考点：迭代器的作用）【简单】

在C++的STL（标准模板库）中，迭代器提供了一种统一的方法来遍历不同容器中的元素，而无需了解容器的内部结构，是容器无关的。
迭代器可以被视为指向容器中某个特定位置的指针，允许程序访问和操作容器中的每个元素。

## 什么是迭代器失效，如何避免？ （考点：迭代器失效）【中等】

迭代器失效（iterator invalidation） 指的是：容器在执行某些修改操作后，原先持有的迭代器 / 指针 / 引用 不再指向有效元素，继续使用将产生未定义行为（UB）。
失效不仅发生在迭代器本身，指向容器元素的指针和引用 也可能一并失效。

### 常见容器的失效规则（速记）

- std::vector / std::basic_string（连续存储）

push_back/insert/emplace/resize 可能触发扩容 → 所有 迭代器/指针/引用失效
即便不扩容：在插入/删除位置 及其之后 的迭代器失效。
erase(pos)：从 pos 起至末尾的迭代器失效；返回指向 被删元素后一个 的新迭代器。

- std::deque
  
在首/尾插入删除可能使全部迭代器失效（实现相关）
中间插入/删除通常使受影响的区段迭代器失效。

- std::list / std::forward_list（链式存储）

插入/拼接，不会使现有元素的迭代器失效。
删除，仅使被删元素的迭代器失效。

- std::map / std::set（有序关联容器，节点式）
  
insert不使已有迭代器失效。
erase仅使被删节点的迭代器失效。

- std::unordered_map / std::unordered_set（哈希容器）
  
rehash / reserve 导致所有迭代器失效。
insert 一般不使已有迭代器失效（除非触发 rehash）。
erase 仅使被删元素的迭代器失效。

### 典型踩坑示例

- 边遍历边删除（vector）

```c++
std::vector<int> v{1,2,3,4};
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0)
        it = v.erase(it);   // 使用 erase 的返回值继续遍历（正确）
    else
        ++it;               // 不删除时正常前进
}
```

- 扩容导致“全失效”（vector）

```c++
auto it = v.begin();
v.push_back(42);  // 可能扩容 → it 失效
// 使用 it 即 UB，需重新获取 v.begin()
```

- 哈希容器 rehash

```c++
auto it = um.begin();
um.reserve(10000); // 可能 rehash → 所有迭代器失效
// it 失效，需重新获取
```

- 删除 list 元素的安全写法

```c++
for (auto it = lst.begin(); it != lst.end(); ) {
    if (need_erase(*it)) it = lst.erase(it); // 仅删当前，其他迭代器稳定
    else ++it;
}
```
### 如何避免迭代器失效（实践指南）

- 使用返回值继续遍历
erase（C++11 起）返回下一个有效迭代器：it = c.erase(it);

- 提前容量规划（连续存储容器）
vector/string 在大量插入前 reserve(n)，减少扩容次数。

- 遍历时尽量不用下标 + 外部缓存迭代器
修改容器后重新获取相关迭代器/指针/引用。
  
- 选择“节点稳定”的容器
频繁插删且需要稳定迭代器：优先list、(unordered_)map/set。

- 注意哈希容器的 rehash
批量插入前 reserve；rehash 后全部迭代器作废。

- 使用“索引遍历”替代“迭代器遍历”（当容器允许且你可控制索引失效）
对 vector 的尾删可用索引；警惕插入/删除导致的元素移动。

- 不要持有临时容器元素的引用/迭代器
返回局部容器的迭代器/引用 → 悬垂。

- 范围 for + 拷贝值
如只读数据且容器会变更结构，考虑先拷贝必要的值到临时数组再处理。

# 面试用总结（30 秒版）

迭代器失效是指容器被修改后，旧的迭代器/指针/引用不再有效。
连续存储（vector/string）在 扩容或中间插入/删除 会使大量迭代器失效； 
节点式容器（list/map/set）插入一般不失效，erase 只使被删元素失效； 
哈希容器在 rehash 时会使 全部 迭代器失效。
规避方法：用 erase 返回值继续遍历、reserve 预容量、修改后重新获取迭代器、必要时选用迭代器稳定的容器。

## vector容器实现与扩充

- 底层实现

vector在堆中分配了一段连续的内存空间来存放元素

- 三个迭代器

first ： 指向的是vector中对象的起始字节位置
last ： 指向当前最后一个元素的末尾字节
end ： 指向整个vector容器所占用内存空间的末尾字节
last - first ： 表示 vector 容器中目前已被使用的内存空间
end - last ： 表示 vector 容器目前空闲的内存空间
end - first ： 表示 vector 容器的容量

- 扩容过程

如果集合已满，在新增数据的时候，就要分配一块更大的内存，将原来的数据复制过来，释放之前的内存，再插入新增的元素。
所以对vector的任何操作，一旦引起空间重新配置，指向原vector的所有迭代器就都失效了。

### size() 和 capacity()

1. 堆中分配内存，元素连续存放，内存空间只会增长不会减少

capacity()，在不分配新内存下最多可以保存的元素个数。
size()，返回当前已经存储数据的个数。

2. 对于vector来说，capacity是永远大于等于size的。

capacity和size相等时，vector就会扩容，capacity变大（翻倍）。

这里涉及到了vector扩容方式的选择，新增的容量选择多少才适宜呢？

1. 固定扩容
机制：每次扩容的时候在原 capacity 的基础上加上固定的容量，比如初始 capacity 为100，扩容一次为 capacity + 20，再扩容仍然为 capacity + 20;
缺点：考虑一种极端的情况，vector每次添加的元素数量刚好等于每次扩容固定增加的容量 + 1，就会造成一种情况，每添加一次元素就需要扩容一次，而扩容的时间花费十分高昂。所以固定扩容可能会面临多次扩容的情况，时间复杂度较高;
优点：固定扩容方式空间利用率比较高。

2. 加倍扩容
机制：每次扩容的时候原 capacity 翻倍，比如初始capcity = 100, 扩容一次变为200, 再扩容变为400;
优点：一次扩容 capacity 翻倍的方式使得正常情况下添加元素需要扩容的次数大大减少（预留空间较多），时间复杂度较低;
缺点：因为每次扩容空间翻倍，而很多空间没有利用上，空间利用率不如固定扩容。

在实际应用中，一般采用空间换时间的策略。

### resize()和reserve()

- resize()：改变当前容器内含有元素的数量(size())，而不是容器的容量。
当resize(len)中len>v.capacity()，则数组中的size和capacity均设置为len;
当resize(len)中len<=v.capacity()，则数组中的size设置为len，而capacity不变;

- reserve()：改变当前容器的最大容量（capacity）
当reserve(len)的len > 当前的capacity()，那么会重新分配一块能存len个对象的空间，然后把之前的对象通过copy construtor复制过来，销毁之前的内存;
当reserve(len)中len <= 当前的capacity()，则数组中的capacity不变，size不变，即不对容器做任何改变。


vector迭代器：由于vector维护的是一个线性区间，所以普通指针具备作为vector迭代器的所有条件，就不需要重载operator+，operator*之类的东西。

## list（链表）

每个元素都是放在一块内存中，他的内存空间可以是不连续的，通过指针来进行数据的访问。
在哪里添加删除元素性能都很高，不需要移动内存，当然也不需要对每个元素都进行构造与析构了，所以常用来做随机插入和删除操作容器
list是一个环状的双向链表，同时它也满足STL对于“前闭后开”的原则，即在链表尾端可以加上空白节点。

迭代器是泛化的指针所以里面重载了->，--，++，*（），等运算符，同时迭代器是算法与容器之间的桥梁，算法需要了解容器的方方面面，于是就诞生了5种关联类型，(这5种类型是必备的，可能还需要其他类型)我们知道算法传入的是迭代器或者指针，算法根据传入的迭代器或指针推断出算法所想要了解的容器里的5种关联类型的相关信息。由于光传入指针，算法推断不出来其想要的信息，所以我们需要一个中间商（萃取器）也就是我们所说的iterator traits类，对于一般的迭代器，它直接提供迭代器里的关联类型值，而对于指针和常量指针，它采用的类模板偏特化，从而提供其所需要的关联类型的值。

```c++
// 针对一般的迭代器类型,直接取迭代器内定义的关联类型
template<class I>
struct iterator_traits {
    typedef typename I::iteratorcategory iteratorcategory;
    typedef typename I::valuetype valuetype;
    typedef typename I::differencetype differencetype;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
};
​// 针对指针类型进行特化,指定关联类型的值
template<class T>
struct iteratortraits<T> {
    typedef randomaccessiteratortag iteratorcategory;
    typedef T value_type;
    typedef ptrdifft differencetype;
    typedef T *pointer;
    typedef T &reference;
};
// 针对指针常量类型进行特化,指定关联类型的值
template<class T>
struct iteratortraits<const T> {
    typedef randomaccessiteratortag iteratorcategory;
    typedef T valuetype;  // valuetye被用于创建变量,为灵活起见,取 T 而非 const T 作为 value_type
    typedef ptrdifft differencetype;
    typedef const T *pointer;
    typedef const T &reference;
};
```
## deque（双端数组）

支持快速随机访问，由于deque需要处理内部跳转，因此速度上没有vector快。

### deque概述

deque是一个双端开口的连续线性空间，其内部为分段连续的空间组成，随时可以增加一段新的空间并链接。

注意:由于deque的迭代器比vector要复杂，这影响了各个运算层面，所以除非必要尽量使用vector；为了提高效率，在对deque进行排序操作的时候，我们可以先把deque复制到vector中再进行排序最后在复制回deque

### deque中控器

deque是由一段一段的定量连续空间构成。一旦有必要在其头端或者尾端增加新的空间，便配置一段定量连续空间，串接在整个deque的头端或者尾端。
好处：避免“vector的重新配置，复制，释放”的轮回，维护连整体连续的假象，并提供随机访问的接口；
坏处：其迭代器变得很复杂；

deque采用一块map作为主控，其中的每个元素都是指针，指向另一片连续线性空间，称之为缓存区，这个区才是用来储存数据的。

deque拥有两个数据成员start与finish迭代器，分别由deque:begin()与deque:end()传回。
//迭代器的关键行为，其中要注意的是一旦遇到缓冲区边缘，可能需要跳一个缓存区

deque数据结构: deque除了维护一个map指针以外，还维护了start与finish迭代器分别指向第一缓冲区的第一个元素，和最后一个缓冲区的最后一个元素的下一个元素，同时它还必须记住当前map的大小。

deque	vector
组织方式	按页或块来分配存储器的，每页包含固定数目的元素	分配一段连续的内存来存储内容
效率	即使在容器的前端也可以提供常数时间的insert和erase操作，而且在体积增长方面也比vector更具有效率	只是在序列的尾端插入元素时才有效率，但是随机访问速度要比deque快
stack && queue
概述：栈与队列被称之为duque的配接器，其底层是以deque为底部架构。通过deque执行具体操作

heap && priority_queue
heap（堆）：
建立在完全二叉树上，分为两种，大根堆，小根堆,其在STL中做priority_queue的助手，即，以任何顺序将元素推入容器中，然后取出时一定是从优先权最高的元素开始取，完全二叉树具有这样的性质，适合做priority_queue的底层
priority_queue:
优先队列，也是配接器。其内的元素不是按照被推入的顺序排列，而是自动取元素的权值排列，确省情况下利用一个max-heap完成，后者是以vector—表现的完全二叉树。
template<class T, class Sequence=vector <T>, class Compare=less<typename Sequence::value_type>>
class priority_queue {
public:

    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_refernece;

protected:

    Sequence c;//底层容器
    Compare comp//容器比较大小标准

public:

    priority_queue() : c() {}
    explicit priority_queue(const Compare &x) : c(), comp(x) {}
    //以下用到的make_heap(),push_heap(),pop_heap()都是泛型算法

    //任何一个构造函数都可以立即在底层产生一个heap
    template<class InputIterator>
    priority_queue(InputIterator first, InputIterator last const Compare &x)
            :c(first, last), comp(x) { make_heap(c.begin(), c.end(), comp); }

    template<class InputIterator>
    priority_queue(InputIterator first, InputIterator last const Compare &x)
            :c(first, last) { make_heap(c.begin(), c.end(), comp); }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    void push(const value_type &x) {
        _STL_TRy {
            c.push_back(X);
            push_heap(c.begin(), c.end(), comp);
        }
        _STL_UNWIND{c.clear()};
    }

    void pop() {
        _STL_TRY {
            pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
        _STL_UNWEIND{c.clear()};
    }
};

// priority_queue无迭代器
map && set的区别和实现原理
一、map 和 set 的核心区别
对比维度	map	set
存储内容	键值对（key-value）	只有键（key）
元素唯一性	key 唯一，value 可重复	key 唯一
常见用途	根据 key 查 value（字典、映射）	去重、有序集合
迭代器解引用	pair<const Key, T>	const Key
是否可修改 key	❌ 不可	❌ 不可
map 是“有序键值映射”，set 是“有序不重复集合”
二、底层实现原理（重点）
1.底层数据结构
map 和 set 底层都是：红黑树（Red-Black Tree）
属于 自平衡二叉搜索树
特点：
树的高度保持在 O(log n)
插入、删除、查找复杂度都是 O(log n)
元素 自动有序
2.为什么 key 是 const？
红黑树的排序依据就是 key
如果允许修改 key：
会破坏二叉搜索树的有序性
导致整棵树逻辑错误
因此：
map 中是 pair<const Key, T>
set 中是 const Key
三、map 和 set 的操作复杂度
操作	map	set
插入	O(log n)	O(log n)
查找	O(log n)	O(log n)
删除	O(log n)	O(log n)
四、map 和 set 的典型使用场景
map 的使用场景
需要 key → value 映射
例如：配置表、统计次数、索引关系
map<string, int> cnt;
cnt["apple"]++;
set 的使用场景
去重
判断元素是否存在
有序集合
set<int> s;
s.insert(3);
s.insert(3); // 自动去重
map && unordered_map的区别
map中元素是一些key-value对，关键字起索引作用，值表示和索引相关的数据。
底层实现：
map底层是基于红黑树实现的，因此map内部元素排列是有序的。
而unordered_map底层则是基于哈希表实现的，因此其元素的排列顺序是杂乱无序的。
map：
优点：
有序性，这是map结构最大的优点，其元素的有序性在很多应用中都会简化很多的操作。
map的查找、删除、增加等一系列操作时间复杂度稳定，都为O(logn )。
缺点：
查找、删除、增加等操作平均时间复杂度较慢，与n相关。
unordered_map：
优点：
查找、删除、添加的速度快，时间复杂度为常数级O(1）。
缺点：
因为unordered_map内部基于哈希表，以（key,value）对的形式存储，因此空间占用率高。
unordered_map的查找、删除、添加的时间复杂度不稳定，平均为O(1)，取决于哈希函数。极端情况下可能为O(n)。
问题：
为什么insert之后，以前保存的iterator不会失效？
因为 map 和 set 存储的是结点，不需要内存拷⻉和内存移动。但是像 vector 在插入数据时如果内存不够会重新开辟一块内存。map 和 set 的 iterator 指向的是节点的指针，vector 指向的是内存的某个位置
为何map和set的插⼊删除效率⽐其他序列容器⾼？
因为 map 和 set 底部使用红黑树实现，插入和删除的时间复杂度是 O(logn)，而向 vector 这样的序列容器插入和删除的时间复杂度是 O(N)
push_back 和 emplace_back 的区别
push_back 用于在容器的尾部添加一个元素。
container.push_back(value);
container 是一个支持 push_back 操作的容器，例如 std::vector、std::list 等，而 value 是要添加的元素的值。
emplace_back 用于在容器的尾部直接构造一个元素。
container.emplace_back(args);
其中 container 是一个支持 emplace_back 操作的容器，而 args 是传递给元素类型的构造函数的参数。与 push_back 不同的是，emplace_back 不需要创建临时对象，而是直接在容器中构造新的元素。
区别：
push_back 接受一个已存在的对象或一个可转换为容器元素类型的对象，并将其复制或移动到容器中。emplace_back 直接在容器中构造元素，不需要创建临时对象。
emplace_back 通常比 push_back 更高效，因为它避免了创建和销毁临时对象的开销。
emplace_back 的参数是传递给元素类型的构造函数的参数，而 push_back 直接接受一个元素。
#include <vector>
#include <string>

int main() {
    std::vector<int> numbers;

    // 使用 push_back
    numbers.push_back(42);

    // 使用 emplace_back
    numbers.emplace_back(3, 4, 5);  // 通过构造函数参数直接构造元素

    return 0;
}
vector的实现原理
std::vector 是C++标准库中的一个动态数组实现，它的实现原理基于数组数据结构。实现通常包含一个指向数组起始位置的指针、数组的大小和容量等信息。
在尾部进行插入和删除操作时，只需调整尾部指针，不需要移动整个数据块。
当元素数量达到当前内存块的容量时，std::vector 会申请一个更大的内存块，将元素从旧的内存块复制到新的内存块，并释放旧的内存块。
由于数组的连续内存结构，通过索引进行访问时可以通过指针运算实现常数时间复杂度的随机访问。
template <class T, class Allocator = std::allocator<T>>
class vector {
private:
    T* elements;  // 指向数组起始位置的指针
    size_t size;   // 当前元素数量
    size_t capacity;  // 当前分配的内存块容量
};
list的实现原理
std::list 是C++标准库中的一个双向链表实现，它的实现原理基于链表数据结构。每个节点包含两个指针，分别指向前一个节点和后一个节点, 以及存储实际数据的部分。
template <class T>
struct Node {
    T data;
    Node* prev;
    Node* next;
};

std::list 维护一个头指针和一个尾指针，它们分别指向链表的第一个和最后一个节点。
在插入和删除操作时，只需调整相邻节点的指针，不需要移动整个数据块。
vector和list的区别
实现上
vector使用动态数组实现。连续的内存块，支持随机访问。在尾部进行插入/删除操作较快，但在中间或头部进行插入/删除可能会涉及大量元素的移动。
list使用双向链表实现。不连续的内存块，不支持随机访问。在任意位置进行插入/删除操作都是常数时间复杂度。
访问上
vector支持通过索引进行快速随机访问。使用迭代器进行访问时，效率较高。
List不支持通过索引进行快速随机访问。迭代器在访问时需要遍历链表，效率相对较低。
插入和删除操作
**vector：**在尾部进行插入/删除操作是常数时间复杂度。在中间或头部进行插入/删除可能涉及大量元素的移动，时间复杂度为线性。
List: 在任意位置进行插入/删除操作都是常数时间复杂度，因为只需调整相邻节点的指针。
内存管理
Vector使用动态数组，需要在预估元素数量时分配一块较大的内存空间。
list由于采用链表结构，动态分配的内存比较灵活。每个元素都有自己的内存块，避免了预分配的问题。
适用场景
Vector适用于需要频繁随机访问、在尾部进行插入/删除操作的场景。
list适用于需要频繁在中间或头部进行插入/删除操作、不要求随机访问的场景。
迭代器有什么作用？什么时候迭代器会失效
迭代器为不同类型的容器提供了统一的访问接口， 隐藏了底层容器的具体实现细节， 允许开发者使用一致的语法来操作不同类型的容器。
对于序列容器vector，deque来说，使用erase后，后边的每个元素的迭代器都会失效，后边每个元素都往前移动一位，erase返回下一个有效的迭代器。
对于关联容器map，set来说，使用了erase后，当前元素的迭代器失效，但是其结构是红黑树，删除当前元素，不会影响下一个元素的迭代器，所以在调用erase之前，记录下一个元素的迭代器即可。
对于list来说，它使用了不连续分配的内存，并且它的erase方法也会返回下一个有效的迭代器，因此上面两种方法都可以使用。






