#include <iostream>

template<int N>
struct sample
{
	void print()
	{
		std::cout<<"value="<<N<<std::endl;
	}
};

#define SAMPLE( Val )                                       
template<>struct sample< Val >                         \
{                                                      \
	void print()                                   \
	{                                              \
		std::cout<<"spec value="<<Val<<std::endl;\
	}                                              \
};

SAMPLE(0)
SAMPLE(1)
SAMPLE(2)
SAMPLE(3)
SAMPLE(4)

int main()
{
	sample<0> s0;
	s0.print();

	sample<1> s1;
	s1.print();

	sample<2> s2;
	s2.print();
}

