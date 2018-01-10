
/**
 * 智能指针的实现，独占控制权
 */
template<class T>
class smart_ptr
{
private:
	T *m_ptr;

	typedef T elem_type;
	typedef smart_ptr<T> this_type;

	//防止对象的拷贝,拷贝构造函数需要私有化
	smart_ptr(const smart_ptr &);//private修饰的构造函数可以不实现

	//防止对象的赋值，赋值运算符函数需要私有化
	this_type& operator=(const this_type &);//private修饰的赋值运算符函数可以不实现

public:
	//使用explicit关键词要求使用显式调用构造函数
	explicit smart_ptr(T* p=0):m_ptr(p)
	{
	}

	//在析构函数中需要执行delete回收内存空间
	~smart_ptr()
	{
		if(m_ptr)
		{
			delete m_ptr;
			m_ptr=0;
		}
	}

	//提供get()访问方法
	elem_type* get() const
	{
		return m_ptr;
	}

	//重载了*运算符，模拟指针功能
	elem_type& operator*() const
	{
		return *m_ptr;
	}

	//重载了->运算符，模拟指针功能
	elem_type* operator->() const
	{
		return m_ptr;
	}

	//提供了swap()方法
	void swap(this_type &other)
	{
		elem_type *p=m_ptr;
		m_ptr=other.m_ptr;
		other.m_ptr=p;
	}

	//提供了reset()方法，其中使用了构造函数构建临时对象和swap()方法实现
	void reset(elem_type * p=0)
	{
		this_type(p).swap(*this);
	}
};

/**
 *定义一个空指针类型nullptr_t，在标准中的
 * C++11中空指针类型已经定义为std::nullptr_t
 */
class nullptr_t
{
public:  
    	template<class T>  
	operator T*() const 
	{
		return 0;
	}  
    	
	template<class C, class T>  
	operator T C::*() const 
	{ 
		return 0; 
	}  

private:  
    void operator& () const;
};
const nullptr_t nullptr = {}; //空指针定义 

template<class T>
inline bool operator==(const smart_ptr<T> &p,nullptr_t)
{
	return (0==p.get());
}

template<class T>
inline bool operator!=(const smart_ptr<T> &p,nullptr_t)
{
	return 0!=p.get();
}

///////////////////////////////////////////////////////////
//测试代码部分
#include<iostream>

class test
{
public:
	test(){std::cout<<"test()"<<std::endl;}
	~test(){std::cout<<"~test()"<<std::endl;}
	void print(){std::cout<<"print()"<<std::endl;}
};

int main(int argc,char *argv[])
{
	smart_ptr<test> ptr(new test());
	if(ptr!=nullptr)
	{
		bool ret=(ptr==nullptr);
		std::cout<<"ret:"<<ret<<std::endl;

		ptr->print();
		ptr.reset();
	}
	std::cout<<"over"<<std::endl;
}
