// g++ -std=c++11 -pthread thread.cpp
#include <iostream>
#include <thread>

void hello(int &param)
{
	for( int i =0;i<param;++i)
		std::cout<<"Counter:"<<i<<std::endl;
	return;
}

int main(int argc,char *argv[])
{
	int param=20;
	std::thread th1(hello,std::ref(param));
	th1.join();
	std::cout<<"over!"<<std::endl;

	return 0;
}
