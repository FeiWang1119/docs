# 55 Specific Ways to Improve Your Programs and Designs 

Table of contents
-----------------
  * [Item 1 View C++ as a federation of languages](#item-1-view-c++-as-a-federation-of-languages)
  * [Item 2 Prefer consts enums and inlines to defines](#item-2-prefer-consts-enums-and-inlines-to-defines)
  * [Item 3 Use const whenever possible](#item-3-use-const-whenever-possible)

Item 1 View C++ as a federation of languages
---------------------------------------------
The easiest way is to view C++ not as a single language but as a federation of related languages.
To make sense of C++, you have to recognize its primary sublanguages. Fortunately, there are
only four:

- C. Way down deep, C++ is still based on C. 

- Object-Oriented C++. This part of C++ is what C with Classes was all
about: classes (including constructors and destructors), encapsulation,
inheritance, polymorphism, virtual functions (dynamic binding), etc.

- Template C++. This is the generic programming part of C++, the one
that most programmers have the least experience with.

- The STL. The STL is a template library, of course, but it's a very special
template library. Its conventions regarding containers, iterators,
algorithms, and function objects mesh beautifully, but templates and
libraries can be built around other ideas, too.

**Things to Remember**

- Rules for effective C++ programming vary, depending on the
part of C++ you are using.

Item 2 Prefer consts enums and inlines to defines
-----------------------------------------------------

This Item might better be called “prefer the compiler to the preprocessor,”
because #define may be treated as if it's not part of the language per se. That's
one of its problems. 
```c
#define ASPECT_RATIO 1.653
```
This can be confusing if you get an error during compilation involving the use of the constant. 
#defines don't respect scope. Which means that not only can't #defines be used for class-specific
constants, they also can't be used to provide any kind of encapsulation, i.e.,
there is no such thing as a “private” #define. 

**Things to Remember**

- For simple constants, prefer const objects or enums to #defines.
- For function-like macros, prefer inline functions to #defines.

Item 3 Use const whenever possible
-----------------------------------

**Things to Remember**

- Declaring something const helps compilers detect usage errors.
const can be applied to objects at any scope, to function parameters and return types,
and to member functions as a whole.
- Compilers enforce bitwise constness, but you should program using logical constness.
- When const and non-const member functions have essentially identical implementations,
code duplication can be avoided by having the non-const version call the const version.


## Item 5: Know what functions C++ silently writes and calls

Things to Remember

- Compilers may implicitly generate a class's default constructor,
copy constructor, copy assignment operator, and destructor.

## Item 6: Explicitly disallow the use of compiler-generated functions you do not want
