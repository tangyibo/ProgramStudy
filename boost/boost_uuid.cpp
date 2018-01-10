//UUID是University Unique Identifier的缩写，它是一个128位的
//数字(16字节),不需要有一个中央认证机构就可以创建全国唯一的
//标示符。别名：GUID 
#include <iostream>
#include <boost/uuid/uuid.hpp>  
#include <boost/uuid/uuid_generators.hpp>  
#include <boost/uuid/uuid_io.hpp>

int main(int argc,char *argv[])
{
	boost::uuids::random_generator rgen;
	boost::uuids::uuid uid=rgen();
	std::cout<<uid<<std::endl;

	return 0;
}
