#include "UrlCoder.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
        string test_str1 = "我的一个URL编码解码测试程序";
       	cout << "test_str1=" << UrlCoder::UrlEncode(test_str1) <<endl;
 
	string test_str2 = "%e6%88%91%e7%9a%84%e4%b8%80%e4%b8%aaURL%e7%bc%96%e7%a0%81%e8%a7%a3%e7%a0%81%e6%b5%8b%e8%af%95%e7%a8%8b%e5%ba%8f";
       	cout << "test_str2=" << UrlCoder::UrlDecode(test_str2) <<endl;

    	return 0;
}
