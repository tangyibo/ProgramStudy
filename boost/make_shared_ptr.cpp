#include<iostream>
#include<string>
#include<list>
#include<boost/smart_ptr.hpp>
#include<boost/make_shared.hpp>

class test
{
public:
	test(int a,std::string s):m_a(a),m_str(s){}
	~test(){}

	void print(){std::cout<<"a="<<m_a<<", str="<<m_str<<std::endl;};

private:
	int m_a;
	std::string m_str;
};

int main()
{
	std::list< boost::shared_ptr<test> > tlist;
	for(int i=0;i<10;++i)
	{
		boost::shared_ptr<test> pt=boost::make_shared<test>(i+1,"hello");
		if(pt)
		{
			tlist.push_back(pt);
		}
	}

	std::list< boost::shared_ptr<test> >::const_iterator it;
	for(it=tlist.begin();it!=tlist.end();++it)
	{
		(*it)->print();
	}

	return 0;
}
