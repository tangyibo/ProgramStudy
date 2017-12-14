/*
 * 编译需要C++支持：
 * g++ -std=c++11 example-05.cpp -o test05
 */

#include <iostream>
#include <memory>   //declarations of std::auto_ptr
#include <utility>  //declarations of std::unique_ptr

class Person
{
public:
	Person(const int i):age(i)
	{
		std::cout<<"Person::Person()"<<std::endl;
	}

	~Person()
	{
		std::cout<<"Person::~Person()"<<std::endl;
	}

	void Print()
	{
		std::cout<<"Person::age="<<age<<std::endl;
	}

public:
	int age;
};

int main(int argc,char *argv[])
{
	std::auto_ptr<Person> p0(new Person(1));
	if(p0.get())
	{
		p0->Print();	
		p0.reset();
	}

	std::unique_ptr<Person> p1(new Person(1));
	if(p1.get())
	{
		p1->Print();
	
		//std::unique_ptr<Person> p2=p1; #不能编译通过
		//p2->Print();
	}
	
	return 0;
}
