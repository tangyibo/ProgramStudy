#include<iostream>

class A
{
public:
	void print()
	{ std::cout<<"A::print()"<<std::endl; }
};

class B:A  //这里等价于class B:private A 即B继承A的方式为私有继承
{
public:
	void test()
	{
		print();
		std::cout<<"B::test()"<<std::endl; 
	}
};

int main()
{

	B b;
	b.test();
	return 0;
}
