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
};

class C:public A
{
	public:
		C(){}
		~C(){}

		void Fun(){std::cout<<"C::Fun()"<<std::endl;}   
};

class D:public A
{
	public:
		D(){}
		~D(){}

		void Fun(){std::cout<<"D::Fun()"<<std::endl;}   
};


int main(int argc,char *argv[])
{
	A* p=new B();
	std::cout<<"A* p=new B();"<<std::endl;
	std::cout<<"typeid(p).name()="<<typeid(p).name()<<std::endl;
	std::cout<<"typeid(*p).name()="<<typeid(*p).name()<<std::endl;
	delete p;


	std::cout<<"A* p=new C();"<<std::endl;
	p=new C();
	std::cout<<"typeid(p).name()="<<typeid(p).name()<<std::endl;
	std::cout<<"typeid(*p).name()="<<typeid(*p).name()<<std::endl;
	delete p;

	std::cout<<"A* p=new D();"<<std::endl;
	p=new D();
	std::cout<<"typeid(p).name()="<<typeid(p).name()<<std::endl;
	std::cout<<"typeid(*p).name()="<<typeid(*p).name()<<std::endl;
	delete p;


	std::cout<<"A* q=new D();A* q1=new D()"<<std::endl;
	A* q=new D();
	A* q1=new D();
	std::cout<<"typeid(q)==typeid(D) is :"<<(typeid(q)==typeid(D))<<std::endl;
	D *s=dynamic_cast<D *>(q);
	std::cout<<"typeid(dynamic_cast<D *>(q))==typeid(D) is :"<<(typeid(*s)==typeid(D))<<std::endl;
	std::cout<<"typeid(q)==typeid(q1) is :"<<(typeid(q)==typeid(q1))<<std::endl;
	A* q2=new B();
	std::cout<<"A* q2=new B();"<<std::endl;
	std::cout<<"typeid(q)==typeid(q2) is :"<<(typeid(q)==typeid(q2))<<std::endl;
	std::cout<<"typeid(*q)==typeid(*q2) is :"<<(typeid(*q)==typeid(*q2))<<std::endl;
	delete q,q1,q2;


	return 0;
}
