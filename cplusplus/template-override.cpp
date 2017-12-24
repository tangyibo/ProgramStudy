/*c++98 : 模板函数重载(多态)*/
#include <iostream>
#include <cstring>

using namespace std;

typedef struct student
{
	char name[5];
	int cls_id;
} student;

/* 常规模板函数 */
template<typename T>
void Swap(T &left,T &right)
{
	T temp=left;
	left=right;
	right=temp;
}

/*显示具体化模板函数*/
template<> 
void Swap(student &left,student &right)
{
	char temp[5];
	strcpy(temp,left.name);
	strcpy(left.name,right.name);
	strcpy(right.name,temp);
}


/*非模板函数*/
void Swap(double &left, double &right)
{
	double temp=left;
	left=right;
	right=temp;
}

/*c++98: 显式实例化模板函数*/
template<int> void Swap(int&,int&);

int main()
{
	int a=1,b=2;
	cout<<"a="<<a<<";b="<<b<<endl;
	Swap(a,b);//调用显式实例化的模板函数
	cout<<"After swap():a="<<a<<";b="<<b<<endl;

	char x='A',y='B';
	cout<<"x="<<x<<";y="<<y<<endl;
	Swap(x,y);//调用隐式实例化模板函数
	cout<<"Afer swap():x="<<x<<";y="<<y<<endl;


	double m=1.1,n=2.2;
	cout<<"m="<<m<<";n="<<n<<endl;
	Swap(m,n);//调用非模板函数
	cout<<"Afer swap():m="<<m<<";n="<<n<<endl;


	return 0;
}
