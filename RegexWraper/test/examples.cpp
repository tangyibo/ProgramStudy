#include "regexhelper.h"
#include <iostream>

using namespace std;
using namespace RegexAPI;
void testregex()
{
		//( 1 )   ((  3  )  2 )((  5 )4)(    6    )
        //(/w+)://((/w+/.)*/w+)((//w*)*)(//w+/./w+)?
        //^协议://网址(x.x...x)/路径(n个/字串)/网页文件(xxx.xxx)
        const char *szReg = "(\\w+)://((\\w+\\.)*\\w+)((/\\w*)*)(/\\w+\\.\\w+)?";
        const char *szStr = "sss http://www.cppprog.com/2009/0112/48.html";

        {    //字符串匹配
            cout <<"match:"<< RegexHelper::IsMatch(szStr,szReg)<<endl;
            //assert(r);
        }

        {    //提取子串
            vector<string> results;
            int total = RegexHelper::Match(szStr,szReg,results);
            cout << "total="<<total<<endl;
            if(total > 0){
                for(vector<string>::const_iterator it = results.begin(); it != results.end(); ++it){
                    cout<< *it <<endl;
                }
            }

        }

        { //查找
            cout<<RegexHelper::Match(szStr,"\\d+")<<endl;

        }

        { //替换
            cout<<RegexHelper::ReplaceFirst(szStr,szReg,"ftp://$2$5")<<endl;
        }
        { //替换2，把<>&转换成网页字符
            string s1 = "(<)|(>)|(&)";
            string s2 = "(?1<)(?2>)(?3&)";
            cout<<RegexHelper::ReplaceFirst("cout << a&b << endl;",s1.c_str(),s2.c_str())<<endl;
            cout<<RegexHelper::ReplaceAll("cout << a&b << endl;",s1.c_str(),s2.c_str())<<endl;

        }

        { //使用迭代器找出所有数字
            vector<string> results;
            int total = RegexHelper::Matches(szStr,"\\d+",results);
            cout << "total="<<total<<endl;
            if(total > 0){
                for(vector<string>::const_iterator it = results.begin(); it != results.end(); ++it){
                    cout<< *it <<endl;
                }
            }
        }

        { //使用迭代器拆分字符串
            vector<string> results;
            int total = RegexHelper::Split(szStr,"/",results);
            cout << "total="<<total<<endl;
            if(total > 0){
                for(vector<string>::const_iterator it = results.begin(); it != results.end(); ++it){
                    cout<< *it <<endl;
                }
            }

        }

        { //使用迭代器拆分字符串2
            vector<string> results;
            // 第一子串和第二子串
            vector<int> subv;subv.push_back(1),subv.push_back(2);
            //取/的前一字符和后一字符（这个字符串形象貌似有点邪恶-_-）
            int total = RegexHelper::Split(szStr,"(.)/(.)",subv,results);
            cout << "total="<<total<<endl;
            if(total > 0){
                for(vector<string>::const_iterator it = results.begin(); it != results.end(); ++it){
                    cout<< *it <<endl;
                }
            }
        }
}

int main(int argc,char *argv[])
{
	testregex();
	return 0;
}
