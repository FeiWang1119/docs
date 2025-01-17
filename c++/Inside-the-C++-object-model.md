# Default Constructor 的构造操作 

nontrival default constructor 的四种情况：

- "有Default constructor"的 Member Class Object(真正需要被调用时才会合成)
- "带有Default constructor"的 Base Class(根据它们的声明顺序合成)
- "带有一个Virtual Function"的 Class(vtbl & vptr)
- "带有一个Virtual Base Class"的 Class(virtual base class pointer/offset)

# Copy Constructor 的构造操作

决定一个copy constructor是否为trival的标准在于class是否展现出所谓的”bitwise copy semantics“。

nontrival copy constructor的四种情况：

- "有copy constructor"的 Member Class Object(显式or编译器合成)
- "带有copy constructor"的 Base Class(显式or编译器合成)
- "带有一个Virtual Function"的 Class(vtbl & vptr)
- "带有一个Virtual Base Class"的 Class(virtual base class pointer/offset)

# 
