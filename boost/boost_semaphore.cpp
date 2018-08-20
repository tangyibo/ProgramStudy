//compile : g++ boost_semaphore.cpp -lboost_thread-mt 
//
#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp> 
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 

using namespace std;
using namespace boost::posix_time;

boost::interprocess::interprocess_semaphore g_semaphore(0);
std::queue<int> g_queue;
boost::mutex g_mutex;


void consumer()
{
	while(true)
	{
#if 1
		if(!g_semaphore.try_wait())
		{
			boost::this_thread::sleep(boost::posix_time::seconds(1));
			continue;
		}
#else
		g_semaphore.wait();
#endif
		
		do{
			boost::unique_lock<boost::mutex> lock ( g_mutex );
			int value=g_queue.front();
			printf("consumer: %d\n",value);
			g_queue.pop();
		}while(0);

	}
}


void producer()
{
	int index=1;
	while(true)
	{
		do{
			boost::unique_lock<boost::mutex> lock ( g_mutex );
			g_queue.push(index);
			printf("producer: %d\n",index++);
			g_semaphore.post();
		}while(0);

		boost::this_thread::sleep(boost::posix_time::seconds(2));	
	}
}

int main(int argc,char *argv[])
{
    boost::thread thread1(boost::bind(&consumer));
    boost::thread thread2(boost::bind(&producer));
    getchar();
}
