//compile: g++ template_factorial.cpp -std=c++11
//
#include <iostream>

template<int N>
class Factorial
{
public:
	enum{
		ret=N*Factorial<N-1>::ret
	};
};

template<>
class Factorial<1>
{
public:
	enum{
		ret=1
	};
};

int main()
{
	std::cout<<Factorial<10>::ret<<std::endl;

	return 0;
}
