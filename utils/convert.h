#ifndef _TOOL_CONVEWRT_HEADER_H_
#define _TOOL_CONVEWRT_HEADER_H_
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::string;

class Convert
{
public:
	/*param @mac : char mac[6]; */
	static string mac_bin2str(const unsigned char *mac)
	{
		char buf[18]={0};
		for (int i = 0; i < 6; i++) 
		{
			if ( i ==5 )
			{
				sprintf(buf+(2*i+i), "%02X", mac[i]);
			}
			else
			{
				sprintf(buf+(2*i+i), "%02X:",mac[i]);
			}
		}

		return string(buf);
	}

	static unsigned char char2dec(const char ch)  
	{  
		switch(ch)  
		{  
			case '0': return 0;  
			case '1': return 1;  
			case '2': return 2;  
			case '3': return 3;  
			case '4': return 4;  
			case '5': return 5;  
			case '6': return 6;  
			case '7': return 7;  
			case '8': return 8;  
			case '9': return 9;  
			case 'a':   
			case 'A': return 10;  
			case 'b':   
			case 'B': return 11;  
			case 'c':  
			case 'C': return 12;  
			case 'd':   
			case 'D': return 13;  
			case 'e':   
			case 'E': return 14;  
			case 'f':  
			case 'F': return 15;      
			default: break;
		}  
		return 0;  
	}  

	/* param @macstr: char str[18],@machex: char mac[6] */
	static int mac_str2bin( char *str,unsigned char *mac)
	{  
		for (int i = 0;i < 6;++i)  
		{  
			mac[i] = (char2dec(str[3*i+0]) & 0x00FF)<<4;  
			mac[i] += char2dec(str[3*i+1]) & 0x00FF;
		}     
	}  

	/* param &ip:char ip[16]; */
	static unsigned int ip_str2int(const char *ipaddr)
	{
		return inet_addr(ipaddr);	
	}

	static string ip_int2str(const unsigned int &ip)
	{
		char temp[16]={0};
		unsigned int v=ntohl(ip);
		sprintf(temp, "%d.%d.%d.%d",  
				(v & 0xff000000) >> 24,  
				(v & 0x00ff0000) >> 16,  
				(v & 0x0000ff00) >> 8,  
				(v & 0x000000ff));  

		return string(temp);  
	}

	static string utc2str(const time_t &t)
	{
		struct tm *p = localtime(&t);
		char szFormat[128];

		sprintf(szFormat, "%d-%d-%d %d:%d:%d", p->tm_year + 1900, p->tm_mon + 1,
				p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

		return string(szFormat);
	}

};

/*
#include <iostream>

int main()
{
	char macstr[18]="2C:2C:00:00:C0:EA";
	std::cout<<"Mac="<<macstr<<std::endl;
	unsigned char machex[6]={0};
	Convert::mac_str2bin(macstr,machex);
	std::cout<<"hex2mac="<<Convert::mac_bin2str(machex)<<std::endl;	
	std::cout<<"================="<<std::endl;
	char ipaddr[]="192.168.72.23";
	std::cout<<"IP="<<ipaddr<<std::endl;
	unsigned int ip=Convert::ip_str2int(ipaddr);
	std::cout<<"intip="<<ip<<std::endl;
	std::cout<<"int2str="<<Convert::ip_int2str(ip)<<std::endl;
	
	return 0;
}
*/

#endif
