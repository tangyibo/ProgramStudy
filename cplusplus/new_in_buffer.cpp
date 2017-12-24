/**
 *在C++中new运算符的功能有两个：
 *（1）常规new运算符：从堆中分别内存块
 *（2）定位new运算符：从特定位置（如内存buffer）中分配内存
 */
#include<iostream>

#define BUFSIZE 512

static char buffer[BUFSIZE];//全局变量默认初始化为0

int main(int argc,char *argv[])
{
	using namespace std;
	char *array=new (buffer) char[10];
	for(size_t i=0;i<10;++i)
	{
		array[i]='a'+i;
	}

	for(size_t i=0;i<10;++i)
		cout<<array[i]<<" ";
	cout<<endl;

	cout<<"Global Buffer:"<<buffer<<endl;

  delete []array;
  
	return 0;
}
