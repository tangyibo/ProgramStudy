#include<iostream>
#include<boost/thread.hpp>
#include<boost/functional.hpp>
#include<boost/bind.hpp>
#include<boost/format.hpp>
#include<boost/smart_ptr.hpp>
#include<boost/make_shared.hpp>
#include<string>
#include<vector>

static int counter=0;
boost::mutex mtx;

void ThreadOneFun(int a,std::string s)
{
        int thread_num=0;
        do
        {
                boost::mutex::scoped_lock locker(mtx);
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
        std::vector< boost::shared_ptr<boost::thread> > thrvec;
        for(int i=0;i<10;++i)
        {
                boost::format fmt("thread-%1%");
                fmt % (i+1);
                boost::function<void ()> fun=boost::bind(ThreadOneFun,10000,fmt.str());

                boost::shared_ptr<boost::thread> thr_ptr=boost::make_shared<boost::thread>(fun);
                thrvec.push_back(thr_ptr);
        }

        for(int i=0;i<10;++i)
                thrvec[i]->join();

        return 0;
}
