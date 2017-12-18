/**
 *熟悉数组名与指针的关系
 *
 */
#include<iostream>

using namespace std;


int main(int argc,char *argv[])
{
	short arr[10]={0,1,2,3,4,5,6,7,8,9};
	short (*pArr)[10];
	pArr=&arr;

	for(int i=0;i<10;++i)
		cout<<(*pArr)[i]<<" "<<endl;


	return 0;
}
