C++反射
一.问题的提出
  问题：如何通过类的名称字符串来生成类的对象。比如有一个类ClassA，那么如何通过类名称字符串”ClassA”来生成类的对象呢？

  定义：那么我们如何解决这个问题呢？我们就可以通过反射来解决这个问题。什么是反射呢？我们来个百度百科较官方的定义：反射是程序可以访问、检测和修改它本身状态或行为的一种能力。有点抽象，我的理解就是程序在运行的过程中，可以通过类名称创建对象，并获取类中声明的成员变量和方法。

  局限：C++本身并不支持反射，反而Java支持反射机制。C++是不支持通过类名称字符串”ClassXX”来生成对象的，也就是说我们可以使用ClassXX* object =new ClassXX; 来生成对象，但是不能通过ClassXX* object=new "ClassXX"; 来生成对象。

二.MFC中的使用
1，  CObject类
    CObject类提供了一些公共服务：包括支持序列化，支持运行时提供类信息，支持动态创建以及支持对象诊断输出等，所以凡是从CObject类继承的类都有这些功能。
2．运行时类信息
    CObject类支持运行时提供其类的信息。通过其中的IsKindOf（）成员函数，可以确定对象是属于指定类还是从指定类派生。
    CObject类支持运行时提供其类的信息，是通过一个很重要的结构完成CRuntimeClass（这是一个结构），CRuntimeClass提供了C++对象的运行时候的类的信息，比如ASCII类名，基类的CRuntimeClass，对象的大小，构造函数等信 息，此外还提供了一组成员函数用来动态创建对象，确定对象的类型等。因此有了对象的大小，构造函数等信息，就可以动态的创建对象了，这需要用结构 CRuntimeClass中的成员函数CreateObject（）。
   另外，结构体CRuntimeClass还维持了指向基类的CRuntimeClass指针，这样，按照类的派生关系就构成了CRuntimeClass 链表。从而通过查找该链表中是否有指定的CRuntimeClass，可以判断给定的类是否从某个类派生。这需要用CRuntimeClass的成员函数 中的IsDerivedFrom（）。通过结构体CRuntimeClass中的静态成员变量m_pNextClass,MFC为每个模块（EXE应用程 序）维护了一个全局的CRuntimeClass链表，这个链表保存在模块的状态中。
    但是要注意，一般并不直接在类中使用CRuntimeClass，而是通过一组宏将CRuntimeClass引入到类中，并添加相应的成员。
3．添加运行时类的信息
   为了添加运行时类信息，需要为类添加CRuntimeClass类型的静态成员函数，保存本类的相关信息，比如对象的大小，基类等等，另外还要添加一些方法操作CRuntimeClass成员等。凡是从CObject类派生的类，都可以通过一组宏自动完成上述任务。
   比如在类的头文件中加入宏DECLARE_DYNAMIC（Class_name），这个宏声明了CRuntimeClass结构体中的静态成员.如果传 入的Class_name为CPerson，则对应的变量名为classCPerson并且返回一个CRuntimeClass指针的虚函数。然后在实现 文件中使用IMPLEMENT_DYNAMIC（派生类名，基类名）宏，这个宏会对DECLARE_DYNAMIC（Class_name）中声明的 CRuntimeClass静态成员惊醒初始化，比如设定类名，对象大小，基类的CRuntimeClass等等。实现文件中的宏 RUNTIME_CLASS（className）的作用是返回类className的CRuntimeClass类型的静态成员的指针。CObject 成员函数IsKindOf（）利用CRuntimeClass静态成员确定具体某个对象是属于特定的类还是从特定的来派生的。这个函数内部调用了结构体 CRuntimeClass中的成员函数IsDerivedFrom()。所以如果这个类支持运行时提供类信息，那么使用带类名的 RUNTIME_CLASS（className）宏可以获取表示该类信息的CRuntimeClass对象。比如
CRuntimeClass * pClass= RUNTIME_CLASS（CPerson）;或者通过对象调用GetRuntimeClass成员，也可以获得该对象所属于类的信息成员。一旦获得了运行时类信息，将其传递到IsKindOf（），就可以确定对象是否属于特定类。
4. 动态创建
   所谓的动态创建不是使用C++的运算符号new在堆中创建对象，而是要从文件中获得某个类的 信息之后，利用这些信息恢复或创建该类的对象。因此在类中应该有一个专门存储这个类的相关信息的记录成员。为了在动态创建对象的时候能够获得待创建对象的 所属的类以及其基类的相关信息，MFC要求，在所有可以动态创建对象的类中，要有一个专门存储这个类的相关信息的记录成员。这就是 CRuntimeClass结构体。
   这样，如果在某个类中有这样一个CRuntimeClass结构体信息表，则在创建这个类的 对象的时候，系统就可以根据类名找到这个类的信息表，同时根据这个结构体中的指针m_pBaseClass依次找到其一系列基类的信息。这样系统就可以根 据这些信息表提供的信息把对象创建出来。
   为了把各个类中的类信息表组织成一个大表，MFC再次使用了链表的结构。该链表把每个类的类信息表作为一个链表项，并用指针m_pNextClass把它 们连接起来，从而组织了一个大的类信息表。于是，要想想使得某个类具有动态创建对象的能力，则该类中必须有类信息链表的声明，而在类的外边应该有类信息链 表的实现。所以MFC把在类中声明类信息表的代码封装到宏DECLARE_DYNCREATE中，而把实现类信息表和链表的代码封装在了宏 IMPLEMENT_DYNCREATE中。
   在需要动态创建一个类的对象的时候，要根据类名到存放类信息的链表中去查找和收集该类的信息，为了达到这个目标，MFC又提供了一个宏 RUNTIME_CLASS（CPerson），这个宏可以按照参数提供的类名，在类信息链表中收集该类的相关信息并调用该类的构造函数来创建对象。 C++不直接支持，但是CObject提供了动态创建对象的能力。为了支持动态创建，需要加入运行时类信息。同时需要在初始化CRuntimeClass 静态成员的时候，把CRuntimeClass结构体中的一个变量m_pfnCreateObject设置为不为（NULL），以表示 CRuntimeClass可以支持动态创建。这样就可以调用CRuntimeClass的CreateObject成员函数创建了。
    要想让一个类有动态创建功能，需要遵循下面的步骤。
（1）从CObject派生这个类。然后在类的声明中使用宏DECLARE_DYNCREATE。该宏内部调用了前面我们用到的宏DECLARE_DYNAMIC（Class_name）。
（2）为这个类定义一个不带参数的构造函数（默认构造函数）。这是因为动态创建过程要使用这个构造函数进行初始化。
（3）在类的实现文件中使用IMPLEMENT_DYNCREATE宏。这个宏除了完成IMPLEMENT_DYNAMIC的功能外，还负责实现DECLARE_DYNCREATE中声明的静态成员CreateObject（）
5 序列化
    序列化是比动态创建更高级的功能，它提供了将对象保存到文件或其他存储设备以及从存储中创建对象的能力。为了支持序列化，也要向类添加运行时信息，添加动 态创建，此外要专门添加针对序列化的代码。CObject用另外一组宏支持序列化。为了简化对象的序列化操作，MFC类库提供了CArchive类，作为 序列化对象和存储介质之间的中间物。
     因此为了支持序列化，必须把类中的结构体CRuntimeClass的静态成员m_wSchema成员设置为非OXFFFF的数值。
     这项设置工作是通过一组宏来实现的，首先在类声明中使用DECLARE_SERIAL宏。该宏除了完成DECLARE_DYNCREATE所做的工作外， 还声明了友元函数operatro>>以支持从ar中创建对象。CArchive充当内存数据和存储之间的直接的中介，利用它可以把内存数 据，如对象轻松的写入持久性的存储设备如文件中，也可以从持久性存储中轻松构造对象。
     CArchive对象提供了一个类型安全缓冲机制，用于将可序列化对象写入CFile对象或从中读取可序列化对象。通常，CFile对象表示磁盘文件；但 是，它也可以表示“剪贴板”的内存文件（CSharedFile文件）。当存档将对象存储到文件（存储设备）的时候，存档先写入对象的 CRuntimeClass信息。然后再写入对象的数据。当另一个存档将对象从文件加载到内存的时候，将首先获得对象的CRuntimeClass，然后 用它动态创建一个对象。将数据序列化到CArchive对象的时候，CArchive对象积累数据，直到它的缓冲区被添满为止。然后，CArchive对 象将其缓冲区写入为其指定的CFile对象。读出的时候，反之。在文档/视图应用框架下，每当用户选择打开文件，新建文件或者保存文件的时候，框架会自动 创建CArchive对象以将文档对象保存到文件或从文件中加栽数据。另外，在其他场合也可能需要CArchive对象，例如，可能要序列化到达或来自剪 贴板的数据，由CSharedFile对象表示。或者可能要使用用户界面来保存与框架提供的文件不同的文件，在这种情况下，可以显式创建CArchive 对象，也就是需要先创建构造CFile对象， 然后将CFile对象传递到CArchive的构造函数。
