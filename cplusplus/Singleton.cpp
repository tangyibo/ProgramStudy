#ifndef __SINGLETON_HEARDER_H__
#define __SINGLETON_HEARDER_H__
#include<mutex>
#include<iostream>

template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		if(NULL==_single_instance)
		{
			static std::mutex locker;
			std::lock_guard<std::mutex> g(locker);

			_single_instance=new(std::nothrow) T();
		}
	
		return *_single_instance;
	}

protected:
	Singleton(){}
	~Singleton(){}

	Singleton(const Singleton &other) {}
	T& operator==(const Singleton &rhs) {}
private:
	static T* _single_instance;
};

class Person:public Singleton<Person>
{
public:
	void print()
	{
		std::cout<<"hello world!"<<std::endl;
	}
};

template<> Person* Singleton<Person>::_single_instance=NULL;

int main(int argc,char *argv[])
{
	Person &p=Person::Instance();
	p.print();
	return 0;
}


#endif
