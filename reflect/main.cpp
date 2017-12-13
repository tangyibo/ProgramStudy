#include <iostream>
#include <string>
#include "Object.h"
using namespace std;

class A : public Object
{
DECLARE_DYNCREATE(A);
public :
	A(){cout<<hex<<(long)this<<" A constructor!"<<endl;}
	~A(){cout<<hex<<(long)this<<" A destructor!"<<endl;}
};
IMPLEMENT_DYNCREATE(A);

class B : public Object
{
DECLARE_DYNCREATE(B);
public :
	B(){cout<<hex<<(long)this<<" B constructor!"<<endl;}
	~B(){cout<<hex<<(long)this<<" B destructor!"<<endl;}
};
IMPLEMENT_DYNCREATE(B);


int main(int argc,char *argv[])
{
	Object* pA = (Object*)Object::CreateInstance("A");
	Object* pB = Object::CreateInstance("B");
	delete pA;
	delete pB;
	Object* p = (Object*)Object::CreateInstance("A");
	delete p;
	
	return 0;
}
