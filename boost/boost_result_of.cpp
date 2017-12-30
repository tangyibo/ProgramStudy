#include<boost/utility/result_of.hpp>
#include<iostream>
#include<cmath>
#include<typeinfo>


int main()
{
	typedef double (*Fun)(double);
	
	Fun f=sqrt;
	boost::result_of<Fun(double)>::type result;
	result=f(4.0);

	std::cout<<"result="<<result<<std::endl;
	std::cout<<typeid(result).name()<<std::endl;

	return 0;
}
