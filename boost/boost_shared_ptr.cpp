#include<boost/smart_ptr.hpp>
#include<iostream>


class Tester
{
public:
	Tester(boost::shared_ptr<int> p):m_p(p)
	{
	}

	~Tester()
	{
	}

	void Print()
	{
		std::cout<<"Counter:"<<m_p.use_count()<<",Value:"<<*m_p<<std::endl;
	}

private:
	boost::shared_ptr<int> m_p;
};

void Print(boost::shared_ptr<int> p)
{
	std::cout<<"Counter:"<<p.use_count()<<",Value:"<<*p<<std::endl;
}

int main(int arg,char *argv[])
{
	boost::shared_ptr<int> sp=boost::shared_ptr<int>(new int);
	std::cout<<"Counter:"<<sp.use_count()<<",Value:"<<*sp<<std::endl;//1,0
	
	*sp=2;

	Tester t(sp);
	t.Print();//2,2

	Print(sp);//3.2

	t.Print();//2,2

	return 0;
}
