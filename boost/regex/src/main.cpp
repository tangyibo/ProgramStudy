#include <iostream>
#include <string>
#include <fstream>
#include "regexhelper.h"
#include "IniParser.h"

using namespace std;

#define _DEBUG_READ_FILE_

void test(const string &cmd)
{
        const string str_oid="SNMPv2-SMI::enterprises.4881.1.1.10.2.81.3.1.1.4.";
        unsigned int start_pos=str_oid.length();
        string regex_format="(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\s{0,1}= Hex-STRING:\\s(\\\S{2})\\s(\\\S{2})\\s(\\\S{2})\\s(\\\S{2})\\s(\\\S{2})\\s(\\\S{2})\\s";
        const int LINE_LENGTH = 1024; 
        char str[LINE_LENGTH];
#ifdef _DEBUG_READ_FILE_
        ifstream fin("./test.txt");
        while( fin.getline(str,LINE_LENGTH) )
	{
#else
        regex_format.append("\n");
	FILE *fstream = popen (cmd.c_str (), "r");
	if(NULL == fstream) 
	{  
                std::cout<<"Execute system command failed for :"<<cmd<<std::endl;
		return;
        } 

	while(true)
	{
		if( NULL== fgets (str,LINE_LENGTH, fstream))
			break;
#endif
                char *pstart=&str[start_pos];
                vector<string> ret;
                int count=RegexHelper::RegexSplit(pstart,regex_format.c_str(),ret);
                if(count<15)
                {
			std::cout<<str<<std::endl;
                        std::cout<<"Maybe error !,cout="<<count<<std::endl;
                }
                else
                {
                        string ap_mac,radio_id,wlan_id,bssid;
                        for(int i=1;i<=6;++i)
                        {
				int val=atoi(ret[i].c_str());
				char buf[32];
				sprintf(buf,"%02X",val);				

                                if(i>1) ap_mac.append(":").append(buf);
                                else ap_mac.append(buf);
                        }

                        radio_id=ret[7];
                        wlan_id=ret[8];

                        for(int i=9;i<=14;++i)
                        {
                                if(i>9) bssid.append(":").append(ret[i]);
                                else bssid.append(ret[i]);
                        }

                        std::cout<<"ap_mac:"<<ap_mac<<"\tradio_id="<<radio_id<<"\twlan_id="<<wlan_id<<"\tbssid="<<bssid<<std::endl;
                }

        }//while

#ifdef _DEBUG_READ_FILE_
	fin.close();
#else
	pclose(fstream);
#endif
}

int main(int argc,char *argv[])
{
        IniParser ini("./config.conf");
        string cmd=ini.ReadProfileString("LBS","command","");
	test(cmd);	

	return 0;
}
