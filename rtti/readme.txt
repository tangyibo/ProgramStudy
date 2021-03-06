RTTI（Run-Time Type Identification，通过运行时类型识别）程序能够使用基类的指针或引用来检查这些指针或引用所指的对象的实际派生类型。面向对象的编程语言，像C++，Java，delphi都提供了对RTTI的支持。RTTI 在C++中并不是什么新的东西，它早在十多年以前就已经出现了。但是大多数开发人员，包括许多高层次的C++程序员对它并不怎么熟悉，更不用说使用 RTTI 来设计和编写应用程序了。
在C++中，RTTI提供了以下两个非常有用的操作符：
（1）typeid操作符，返回指针和引用所指的实际类型；
（2）dynamic_cast操作符，将基类类型的指针或引用安全地转换为派生类型的指针或引用。

如何确定对象的动态类型呢？答案是使用内建的 RTTI 中的运算符：typeid 和 dynamic_cast。
在C++中存在虚函数，也就存在了多态性，对于多态性的对象，在程序编译时可能会出现无法确定对象的类型的情况。当类中含有虚函数时，其基类的指针就可以指向任何派生类的对象，这时就有可能不知道基类指针到底指向的是哪个对象的情况，类型的确定要在运行时利用运行时类型标识做出。为了获得一个对象的类型可以使用typeid函数，该函数反回一个对type_info类对象的引用，要使用typeid必须使用头文件<typeinfo>。

