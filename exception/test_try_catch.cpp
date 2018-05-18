#include <iostream>
#include <exception>

using namespace std;

class Test
{
public:
	Test()
	{
		std::cout<<"Test::Test()"<<std::endl;
	}

	~Test()
        {
                std::cout<<"Test::~Test()"<<std::endl;
        }
};

class MyException : public std::exception
{
public:
	MyException(const char *m):std::exception(),msg(m)
	{
	}

	virtual const char* what() const throw()
	{
		return this->msg;
	}

private:
	const char *msg;
};

int main(int argc,char *argv[])
{
	try{
		Test t;
		//在try中创建的栈对象，用throw抛出异常时，会调用该栈对象的析构函数；
		//但是如果是创建的堆对象，即Test *t=new Test()，用throw抛出异常时，析
		//构函数将不会被调用.所以，为了安全，这里常常使用智能指针。
		throw MyException("hello");
	}
	catch(std::exception &e)
	{
		std::cout<<"error:"<<e.what()<<std::endl;
	}

	std::cout<<"over!"<<std::endl;

	return 0;
}
