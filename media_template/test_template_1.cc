#include <iostream>

template<typename T>
class aTMP
{
public:
	typedef const T reType;
};

class Base
{
public:
	template<int N>
	void fun()
	{
		std::cout<<"member:"<<N<<std::endl;
	}
};

template<typename T>
class Derived : public Base
{
public:
	typename T::reType m;
	
	Derived(typename T::reType a):m(a){}

	void df1()
	{ 
		return fun<212>(); 
	}

};

int main()
{
	Derived< aTMP<int> > x(10);
	x.df1();
	
	return 0;
}

