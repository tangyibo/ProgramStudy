#include<iostream>
#include<sstream>
#include<boost/progress.hpp>

/**
* Test How to use boost::pgress_timer;
*/
void test_progress_timer()
{
	std::stringstream ss;
	{
		boost::progress_timer pt(ss);
		sleep(2);
	}
	std::cout<<ss.str()<<std::endl;
}

/**
*Test how to use boost:pgress_display;
*/
void test_progress_display()
{
        boost::progress_display pd(10);
        for(int i=0;i<10;++i)
        {
                sleep(1);
                ++pd;
        } 
}

void test_progress_display2()
{
        boost::progress_display pd(10);
        for(int i=0;i<10;++i)
        {
                sleep(1);
		pd.restart(10);
                pd+=(i+1);
        }
}


int main()
{
	test_progress_display2();

	return 0;
}
