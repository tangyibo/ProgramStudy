#include<iostream>
#include<boost/shared_ptr.hpp>

class base
{
public:
	base(){}
	~base(){}

	//虚函数
	virtual void print()
	{
		std::cout<<"base::print()"<<std::endl;
	}

	//普通函数
	void test()
	{
		std::cout<<"base::test()"<<std::endl;
	}
};

class derived:public base
{
public:
	derived(){}
	~derived(){}

	//重载父类的虚函数override
	virtual void print()
	{
		std::cout<<"derived::print()"<<std::endl;
	}

	//覆盖父类的虚函数
        void test()
        {
                std::cout<<"derived::test()"<<std::endl;
        } 

	//子类特有的函数
	void myprint()
	{
		std::cout<<"derived::myprint()"<<std::endl;
	}
};

int main(int argc,char *argv[])
{
	boost::shared_ptr<base> pb(new derived());
	if(pb)
	{
		pb->print();

		boost::shared_ptr<derived> p=boost::dynamic_pointer_cast<derived>(pb);
		p->myprint();

		boost::shared_ptr<base> pp=boost::static_pointer_cast<base>(p);
		pp->test();
	}

	return 0;
}
