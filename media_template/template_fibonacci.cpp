//compile: g++ template_fibonacci.cpp -std=c++11
//
#include <iostream>

#if 0

template<int N>
class Fibonacci
{
public:
	enum{
		ret=Fibonacci<N-1>::ret+Fibonacci<N-2>::ret
	};
};

template<>
class Fibonacci<1>
{
public:
	enum{
		ret=1
	};
};

template<>
class Fibonacci<0>
{
public:
        enum{
                ret=0
        };
};

#else

template<int N>
class Fibonacci
{
public:
       static const int ret=Fibonacci<N-1>::ret+Fibonacci<N-2>::ret;
};

template<>
class Fibonacci<1>
{
public:
       static const int ret=1;
};

template<>
class Fibonacci<0>
{
public:
        static const int ret=0;
};


#endif

int main()
{
	std::cout<<"Fibonacci(10)="<<Fibonacci<10>::ret<<std::endl;
	return 0;
}
