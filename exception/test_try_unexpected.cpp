#include <iostream>
#include <exception>

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

void on_unexpected()
{
	cout<<"unexpected function called!"<<endl;
	throw 1;
}

void test_function() throw (int)
{
	throw MyException("hello");
}

int main(int argc,char *argv[])
{
	std::set_unexpected(on_unexpected);  
	std::cout<<"start!"<<std::endl;
	try{
		test_function();
	}
	catch(int &x)
	{
		std::cout<<"int catch call"<<std::endl;
	}
	catch (...) 
	{ 
		std::cerr << "caught some other exception type\n"; 
	}

	std::cout<<"over!"<<std::endl;

	return 0;
}
