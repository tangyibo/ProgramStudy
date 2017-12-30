#include<iostream>
#include<boost/bind.hpp>
#include<boost/function.hpp>
#include<boost/typeof/typeof.hpp>
#include<string>

int print(int x,std::string str)
{
	std::cout<<"x="<<x<<",str="<<str<<std::endl;
	return 0;
}

struct Tester
{
	int operator()(int x,const char* str)
	{
		std::cout<<"x="<<x<<",str="<<str<<std::endl;
	}
};

int main(int argc,char *argv[])
{
	BOOST_AUTO(fun1,boost::bind(print,1,_1));
        fun1("hello!");

	boost::function<int(int,std::string)> f=print;
	BOOST_AUTO(fun2,boost::bind(f,2,_1));
	fun2("world!");

	BOOST_AUTO(fun3,boost::bind<int>(Tester(),_1,_2));
	fun3(3,"test!");

	return 0;
}
