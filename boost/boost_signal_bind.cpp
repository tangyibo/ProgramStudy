//File : boost_siganl_bind.cpp
//g++ boost_signal_bind.cpp -lboost_signals
#include<boost/bind.hpp>
#include<boost/function.hpp>
#include<boost/signal.hpp>
#include<iostream>

int FunA(const int a,const char *str)
{
	std::cout<<"FunA():a="<<a<<",str="<<str<<std::endl;
}

int FunB(const int a,const char *str)
{
        std::cout<<"FunB():a="<<a<<",str="<<str<<std::endl;
}


class Tester
{
public:
	Tester(){}
	~Tester(){}

	void ResetFun(boost::function<int(const int,const char*)> f)
	{
		m_sig.disconnect_all_slots();
		m_sig.connect(f);
	}

	void CallFun( const int a,const char* s)
	{
		m_sig(a,s);
	}

private:
	boost::signal<int (const int,const char *)>m_sig;
};

int main(int argc,char *argv[])
{
	Tester t;

	t.ResetFun(FunA);
	boost::bind(&Tester::CallFun,&t,_1,_2)(1,"hello");

	t.ResetFun(FunB);
	boost::bind(&Tester::CallFun,&t,_1,_2)(2,"world");

	return 0;
}
