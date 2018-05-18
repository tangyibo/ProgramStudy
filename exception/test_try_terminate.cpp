#include <iostream>
#include <exception>
#include <cstdlib>        // std::abort

using namespace std;

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

void on_terminate()
{
	cout<<"terminate function called!"<<endl;
	abort();
}

int main(int argc,char *argv[])
{
	std::set_terminate(on_terminate);  
	std::cout<<"start!"<<std::endl;
	try{
		throw MyException("hello");
	}
	catch(int &x)
	{
		std::cout<<"int catch call"<<std::endl;
	}

	std::cout<<"over!"<<std::endl;

	return 0;
}
