#include<boost/lexical_cast.hpp>
#include<string>
#include<iostream>

template<class T>
class outable
{
private:
	int ta;
public:
	friend std::ostream& operator<<(std::ostream &os,const T &x)
	{
		os<<typeid(T).name();
		return os;
	}
};

class demo:public outable<demo>
{
	
};

int main()
{
	try{
	int aa=boost::lexical_cast<int>("121");
	float bb=boost::lexical_cast<float>("43.2");
	double cc=boost::lexical_cast<double>("3.141592653");
	long dd=boost::lexical_cast<long>("5453533");

	std::string str1=boost::lexical_cast<std::string>(132131);
	std::string str2=boost::lexical_cast<std::string>(0x10);
	std::string str3=boost::lexical_cast<std::string>(0.618);	 

	demo d;
	std::string str4=boost::lexical_cast<std::string>(d);
	std::cout<<str4<<std::endl;
	}
	catch(boost::bad_lexical_cast &e)
	{
		std::cout<<e.what()<<std::endl;
	}

	return 0;
}
