#include "base64.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc ,const char *argv[])
{
	string plain="hello world 中国!";
	string encode=Base64::encode64(plain);
	cout<<encode<<endl<<"==========="<<endl;
	cout<<Base64::decode64(encode)<<endl;

	return 0;
}

