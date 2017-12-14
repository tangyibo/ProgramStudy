#include <stdio.h>

class BaseClass
{
public:
	BaseClass()
	{
		printf("BaseClass::construction\n");
	}
	~BaseClass()
	{
		printf("BaseClass::destruction\n");
	}
};

class DerivedClass:public BaseClass
{
public:
	DerivedClass()
	{
		printf("DerivedClass::construction\n");
	}
	
	~DerivedClass()
	{
		printf("DerivedClass::destruction\n");
	}
};

int main(int argc,char *argv[])
{
	BaseClass *obj=new DerivedClass();
	delete obj;

	return 0;
}
