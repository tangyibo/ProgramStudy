#include <stdio.h>

//一个空类
class EmptyClass
{
};

//只含有一个int成员变量的类
class MemberClass
{
public:
	int _member;
};

//含有虚函数的类
class BaseClass
{
public:
	BaseClass(){}
	virtual ~BaseClass(){}
};

int main(int argc,char *argv[])
{
	printf("sizeof(char)=%d\n",sizeof(char));
	printf("sizeof(int)=%d\n",sizeof(int));
	printf("sizeof(short)=%d\n",sizeof(short));
	printf("sizeof(long)=%d\n",sizeof(long));
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("sizeof(long long)=%d\n",sizeof(long long));
	printf("sizeof(void *)=%d\n",sizeof(void *));
	printf("sizeof(EmptyClass)=%d\n",sizeof(EmptyClass));
	printf("sizeof(MemberClass)=%d\n",sizeof(MemberClass));
	printf("sizeof(BaseClass)=%d\n",sizeof(BaseClass));
	return 0;
}
