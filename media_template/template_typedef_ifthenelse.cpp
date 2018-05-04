#include <iostream>
#include <typeinfo>

template<bool B,class T1,class T2>
class IfThenElse
{
public:
	typedef T1 ret_type;
};

template<class T1,class T2>
class IfThenElse<true,T1,T2>
{
public:
        typedef T1 ret_type;
};

template<class T1,class T2>
class IfThenElse<false,T1,T2>
{
public:
        typedef T2 ret_type;
};


int main()
{
	IfThenElse< 1+3==2, short,double>::ret_type ret;
	if(typeid(ret)==typeid(short))
		std::cout<<"short type"<<std::endl;
	else
		std::cout<<"double type"<<std::endl;

	return 0;
}
