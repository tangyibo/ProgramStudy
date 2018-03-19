#ifndef _TOOL_CONVEWRT_HEADER_H_
#define _TOOL_CONVEWRT_HEADER_H_
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <math.h> 
#include <stdint.h>

using std::string;

class Convert
{
public:
	static uint16_t swap_2(uint16_t v)
	{
		uint8_t arr[2];
		memcpy(arr,&v,2);
		uint8_t byte;

		byte = arr[0];
		arr[0] = arr[1];
		arr[1] = byte;
		return *(uint16_t*)arr;	
	}

	static uint32_t swap_4(uint32_t v)
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

	static uint64_t swap_8(uint64_t v)
	{
		uint32_t low = (uint32_t) (v & 0x00000000FFFFFFFFLL);
		uint32_t high = (uint32_t) ((v & 0xFFFFFFFF00000000LL) >> 32);

		low = swap_4(low);
		high = swap_4(high);

		return  (uint64_t) high + (((uint64_t) low) << 32);
	}

	static bool is_little_endian()
	{
		int temp=1;
		uint8_t byte_order=((uint8_t *)&temp)[0];
		return (byte_order==1);
	}


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
	static void mac_str2bin(const char *str,unsigned char *mac)
	{  
		for (int i = 0;i < 6;++i)  
		{  
			mac[i] = (char2dec(str[3*i+0]) & 0x00FF)<<4;  
			mac[i] += char2dec(str[3*i+1]) & 0x00FF;
		}     
	}  

	/* param &ip:char ip[16]; */
	static unsigned int ip_str2int(const char *ipstr)
	{
		//return inet_addr(ipaddr);	
		if (ipstr == NULL) return 0; 

		unsigned int i = 3, total = 0, cur; 
		char *str=const_cast<char *>(ipstr);
		char *token = strtok(str, "."); 

		while (token != NULL) 
		{ 
			cur = atoi(token); 
			if (cur >= 0 && cur <= 255) 
			{ 
				total += cur * pow(256, i); 
			}
 
			i --; 
			token = strtok(NULL, "."); 
		} 

		if(is_little_endian())
			total=swap_4(total);
		
		return total; 
	}

	static string ip_int2str(const unsigned int &ip)
	{
		//struct sockaddr_in addr; 
		//addr.sin_addr.s_addr = htonl(ip); 
		//return string(inet_ntoa(addr.sin_addr));	

		char temp[16]={0};
		unsigned int v=ip;

		if(is_little_endian())
			v=swap_4(v);

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
