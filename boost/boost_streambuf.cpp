//g++ boost_streambuf.cpp  -lboost_system-mt -lboost_thread-mt
#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace std;

struct person
{
	char name[16];
	int age;
	int sax;
	int tex;
};

int main()
{
	boost::asio::streambuf buf;
	std::ostream out(&buf);

	struct person p;
	strcpy(p.name,"zhang");
	p.age=67;
	p.sax=127;
	p.tex=259;

	out.write(p.name,16);
	out.write(reinterpret_cast<char *>(&p.age),sizeof(int));
	out.write(reinterpret_cast<char *>(&p.sax),sizeof(int));

	boost::asio::streambuf::const_buffers_type cbt = buf.data();  
	std::vector<char> obj(boost::asio::buffers_begin(cbt), boost::asio::buffers_end(cbt));  
	for(int i=0;i<obj.size();++i)
		std::cout<<"value:"<<obj[i]<<std::endl;

	return 0;
}
