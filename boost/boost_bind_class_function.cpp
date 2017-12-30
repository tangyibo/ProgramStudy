#include<iostream>
#include<boost/bind.hpp>
#include<boost/typeof/typeof.hpp>
#include<string>

class Test
{
public:
	int print(int x,std::string str)
	{
		std::cout<<"x="<<x<<",str="<<str<<std::endl;
		return 0;
	}
};

int main(int argc,char *argv[])
{
	Test t;
	BOOST_AUTO(fun,boost::bind(&Test::print,&t,332,_1));
	fun("hello,world!");

	return 0;
}
