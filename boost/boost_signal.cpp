//File : boost_siganl.cpp
//g++ boost_signal.cpp -lboost_signals
#include<boost/signal.hpp>
#include<iostream>

int print(const char *str)
{
	std::cout<<"Msg:"<<str<<std::endl;
}

int main()
{
	boost::signal<int (const char *str)> sig;
	sig.connect(print);
	sig("hello world!");

	return 0;
}
