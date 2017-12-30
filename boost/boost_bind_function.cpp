#include<iostream>
#include<boost/bind.hpp>
#include<boost/typeof/typeof.hpp>
#include<string>

int print(int x,std::string str)
{
	std::cout<<"x="<<x<<",str="<<str<<std::endl;
	return 0;
}

int main(int argc,char *argv[])
{
	BOOST_AUTO(fun,boost::bind(&print,332,_1));
	fun("hello,world!");

	return 0;
}
