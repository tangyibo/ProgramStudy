#include<typeinfo>
#include<iostream>

class A
{
	public:
		A(){}	
		~A(){}

		virtual void Fun(){std::cout<<"A::Fun()"<<std::endl;}
};

class B:public A
{
	public:
		B(){}
		~B(){}

		void Fun(){std::cout<<"B::Fun()"<<std::endl;}
		void BPrint(){std::cout<<"B::BPrint()"<<std::endl;}
};

class C:public A
{
	public:
		C(){}
		~C(){}

		void Fun(){std::cout<<"C::Fun()"<<std::endl;}   
		void CPrint(){std::cout<<"C::CPrint()"<<std::endl;}
};

class D:public A
{
	public:
		D(){}
		~D(){}

		void Fun(){std::cout<<"D::Fun()"<<std::endl;}   
		void DPrint(){std::cout<<"D::DPrint()"<<std::endl;}
};


int main(int argc,char *argv[])
{
	A* p=new B();
	//p->BPrint(); 编译不能通过
	dynamic_cast<B *>(p)->BPrint();

	delete p;

	return 0;
}
