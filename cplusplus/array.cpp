#include<iostream>
#include<array> //using c++11 array template

using namespace std;

int sum(const int *begin,const int *end)
{
	if(end<begin)
		return 0;

	int total=0;
	for(const int *p=begin;p!=end;++p)
	{
		total+=*p;
	}

	return total;
}

void print(array<string,4> arr)
{
	for(int i=0;i<4;++i)
		cout<<arr.at(i)<<" ";
	cout<<endl;
}


int main(int argc,char *argv[])
{
	array<int,10>arr;  //int arr[10];
	for(int i=0;i<10;++i)
		arr[i]=i;

	cout<<"total="<<sum(arr.begin(),arr.end())<<endl;

	array<string,4> names={"Join","Lucy","Jack","Twin"};
	print(names);
	
	return 0;
}
