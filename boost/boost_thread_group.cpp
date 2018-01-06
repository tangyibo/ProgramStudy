#include<iostream>
#include<boost/thread.hpp>
#include<boost/functional.hpp>
#include<boost/bind.hpp>
#include<boost/format.hpp>
#include<string>

static int counter=0;
boost::mutex mtx;

void ThreadOneFun(int a,std::string s)
{
        int thread_num=0;
        do
        {
                boost::mutex::scoped_lock l(mtx);
                ++counter;
                thread_num=counter;
        }while(0);


        boost::this_thread::sleep(boost::get_system_time()+boost::posix_time::seconds(1));

        for(int i=0;i<a;++i)
        {
                std::cout<<"["<<thread_num<<":"<<s<<"]"<<"-"<<i<<std::endl;
        }
}

int main(int argc,char *argv[])
{
	boost::thread_group thr_pool;
        for(int i=0;i<10;++i)
        {
                boost::format fmt("thread-%1%");
                fmt % (i+1);
                boost::function<void ()> fun=boost::bind(ThreadOneFun,100,fmt.str());

		thr_pool.create_thread(fun);
        }

	std::cout<<"Thread Group Size :"<<thr_pool.size()<<std::endl;
	thr_pool.join_all();

        return 0;
}

