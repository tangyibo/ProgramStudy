#include<boost/format.hpp>
#include<iostream>
#include<string>

int main(int arg,char *argv[])
{
	//(1) 接近 printf 的用法
	std::string s=std::string("hello");
	boost::format fmt1("%s-%d-%c");
	fmt1%s%10%'H';
	std::string str=fmt1.str();
	std::cout<<str<<std::endl;

	//(2) %N%占位符的使用
	boost::format fmt("select * from %1% where userid=%2%");
	std::cout<<(fmt%"test"%121)<<std::endl;

	//(3) 占位符顺序问题 
	std::cout<<boost::format("%2% is %1%")%1%"this"<<std::endl;

	
	return 0;
}
