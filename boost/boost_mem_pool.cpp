//compile:g++ boost_mem_pool.cpp -lboost_system -lboost_thread
#include<iostream>
#include<string>
#include<boost/pool/pool.hpp>
#include<boost/pool/object_pool.hpp>

void test_boost_pool()
{
	boost::pool<> pl(sizeof(int));
	int *pi=static_cast<int *>(pl.malloc());
	if(pl.is_from(pi))
	{
		std::cout<<"pl.is_from(pi)"<<std::endl;
		*pi=20;
		pl.free(pi);
	}

	for(int i=0;i<10;++i)
	{
		pl.ordered_malloc(2);
	}
}

class demo
{
	public:
		demo(const int a,const std::string &s)
			:m_val(a),m_str(s){}

		~demo()
		{
			std::cout<<"~demo()"<<std::endl;
		}

		void print()
		{
			std::cout<<"m_a="<<m_val<<",m_str="<<m_str<<std::endl;
		}
	private:
		int m_val;
		std::string m_str;
};

void test_boost_object_pool()
{
	boost::object_pool<demo> objpl;
	demo *pd1=objpl.construct(2,"hello");
	pd1->print();
}

int main()
{
	//test_boost_pool();
	test_boost_object_pool();
	return 0;
}
