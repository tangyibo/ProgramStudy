#ifndef _SERIALIZE_HEADER_H_
#define _SERIALIZE_HEADER_H_
#include <sstream>   //std::stringstream
#include <stdint.h>  //uint8_t uint16_t uint32_t uint64_t
#include <string.h>  //memcpy

////////////////////////////////////////////////
//First,Tools function for bytes order in socket
////////////////////////////////////////////////
uint16_t swap_2(uint16_t v)
{
	uint8_t arr[2];
	memcpy(arr,&v,2);
	uint8_t byte;

	byte = arr[0];
	arr[0] = arr[1];
	arr[1] = byte;
	return *(uint16_t*)arr;	
}

uint32_t swap_4(uint32_t v)
{
	uint8_t arr[4];
	memcpy(arr,&v,4);
	uint8_t byte;

	byte = arr[0];
	arr[0] = arr[3];
	arr[3] = byte;

	byte = arr[1];
	arr[1] = arr[2];
	arr[2] = byte;

	return *(uint32_t*)arr;
}

uint64_t swap_8(uint64_t v)
{
	uint32_t low = (uint32_t) (v & 0x00000000FFFFFFFFLL);
	uint32_t high = (uint32_t) ((v & 0xFFFFFFFF00000000LL) >> 32);

	low = swap_4(low);
	high = swap_4(high);

	return  (uint64_t) high + (((uint64_t) low) << 32);
}

bool is_little_endian()
{
	int temp=1;
	uint8_t byte_order=((uint8_t *)&temp)[0];
	return (byte_order==1);
}

////////////////////////////////////////////////////
// Second,define normal template function
////////////////////////////////////////////////////

template<typename SerializableType>
std::string serialize(SerializableType& a)
{
	return a.serialize();
}

template<typename SerializableType>
int deserialize(std::string &str,SerializableType& a)
{
	return a.deserialize(str);
}

/////////////////////////////////////////////////
//Third,define special template function
//Serialize for C/C++ basic type
//examples: short,int,float,long long,double
/////////////////////////////////////////////////
#define DEF_BASIC_TYPE_SERIALIZE(Type) \
 template<> \
std::string serialize(Type& b) \
{ \
	std::string ret; \
	Type a=b; \
	if(is_little_endian()) \
	{ \
		size_t size=sizeof(Type);\
		switch(size) \
		{ \
			case 2:a=swap_2(a);break; \
			case 4:a=swap_4(a);break; \
			case 8:a=swap_8(a);break; \
			default:break; \
		}; \
	}\
	ret.append((const char*)&a,sizeof(Type)); \
	return ret; \
}

#define DEF_BASIC_TYPE_DESERIALIZE(Type)  \
 template<> \
int deserialize(std::string& str,Type& b)\
{ \
	Type a; \
	memcpy(&a,str.data(),sizeof(Type)); \
        if(is_little_endian()) \
        { \
                size_t size=sizeof(Type);\
                switch(size) \
                { \
                        case 2:a=swap_2(a);break; \
                        case 4:a=swap_4(a);break; \
                        case 8:a=swap_8(a);break; \
                        default:break; \
                }; \
        } \
	b=a; \
	return sizeof(Type); \
}

#define DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(Type) \
	DEF_BASIC_TYPE_SERIALIZE(Type) \
	DEF_BASIC_TYPE_DESERIALIZE(Type)

DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(char)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned char)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(short int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned short int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(float)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(long long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(unsigned long long int)
DEF_BASIC_TYPE_SERIALIZE_AND_DESERIALIZE(double)

//////////////////////////////////////
//Fourth,Serialize for type string
/////////////////////////////////////

// for c++ type std::string
template<>
std::string serialize(std::string& s)
{
	int len=s.size()+1;
	std::string ret;
	ret.append(::serialize(len));
	ret.append(s.data(),len);
	return ret;
}

template<>
int deserialize(std::string &str,std::string& s)
{
	int len;
	::deserialize(str,len);
	s=str.substr(sizeof(len),len);
	return sizeof(int)+len;
}

////////////////////////////////////////////
//Fifth, define input and output stream
//for serialize data struct
////////////////////////////////////////////

class OutStream
{
public:
	OutStream():os(std::ios::binary){}

	template<typename SerializableType>
	OutStream& operator << (SerializableType& a)
	{
		std::string x=::serialize(a);
		os.write(x.data(),x.size());
		return *this;
	}

	std::string str()
	{
		return os.str();
	}

public:
	std::ostringstream os;
};

class InStream
{
public:
	InStream(std::string &s):str(s),total(s.size()) {}
	
	template<typename SerializableType>
	InStream& operator >> (SerializableType& a)
	{
		int ret=::deserialize(str,a);
		str=str.substr(ret);
		return *this;
	}

	int size()
	{
		return total-str.size();	
	}
	
protected:
	std::string str;
	int total;
};

////////////////////////////////////////////
//Sixth,Serialize for custom class object
//If your class object want to be serialized,
//Plese derive for this base class
///////////////////////////////////////////
class Serializable
{
public:
    virtual std::string serialize()=0;
    virtual int deserialize(std::string&)=0;
};


//////////////////////////////////////////
//Example
/////////////////////////////////////////

/*@code
#include "serialize.h"
#include <iostream>

class MyTest :public Serializable
{
public:
	std::string m_name;
	int m_age;
	float m_salary;

public:
	MyTest()
	{
		m_name.clear();
		m_age=0;
		m_salary=0.;
	}

	MyTest(const char *name,const int age,const float salary)
	{
		m_name.assign(name);
		m_age=age;
		m_salary=salary;
	}

	virtual std::string serialize()
	{
		OutStream os;
		os<<m_name<<m_age<<m_salary;
		return os.str();
	}

	virtual int deserialize(std::string &str)
	{
		InStream is(str);
		is>>m_name>>m_age>>m_salary;
		return is.size();
	}

	void display()
	{
		std::cout<<m_name<<","<<m_age<<","<<m_salary<<std::endl;
	}
}; 

OutStream& operator<<(OutStream& os, MyTest &obj)
{
	os<<obj.m_name<<obj.m_age<<obj.m_salary;
	return os;	
}

InStream& operator>>(InStream& is, MyTest &obj)
{
	is>>obj.m_name>>obj.m_age>>obj.m_salary;
	return is;	
}

int main(int argc,char *argv[])
{
	short a=1;
	int b=2;
	long c=3;
	float d=4;
	long long e=5;
	std::string f="hello";
	MyTest t("zhang",23,3200.5);
	MyTest m("lucy",44,2332.2);

	OutStream os;
	os<<a<<b<<c<<d<<e<<f<<t<<m;
	std::string serializestr=os.str();
	
	short a1;
	int b1;
	long c1;
	float d1;
	long long e1;
	std::string f1;
	MyTest t1;
	MyTest m1;
	InStream is(serializestr);
	
	is>>a1>>b1>>c1>>d1>>e1>>f1>>t1>>m1;
	std::cout<<"a="<<a<<",a1="<<a1<<std::endl;
	std::cout<<"b="<<b<<",b1="<<b1<<std::endl;
	std::cout<<"c="<<c<<",c1="<<c1<<std::endl;
	std::cout<<"d="<<d<<",d1="<<d1<<std::endl;
	std::cout<<"e="<<e<<",e1="<<e1<<std::endl;
	std::cout<<"f="<<f<<",f1="<<f1<<std::endl;
	t1.display();
	m1.display();
	
	return 0;
}
*/

#endif
