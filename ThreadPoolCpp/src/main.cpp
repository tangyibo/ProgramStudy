#include "ThreadPool.h"
#include <iostream>
using namespace std;

const int MAX_TASKS = 65535000;

void hello(void* arg)
{
	int* x = (int*) arg;
	cout << "Hello " << *x << endl;
	delete x;
}

int main(int argc, char* argv[])
{
	CMyThreadPool tp;
	int ret = tp.Initialize(15);
	if (ret == -1) 
	{
		cerr << "Failed to initialize thread pool!" << endl;
		return 0;
	}

	for (int i = 0; i < MAX_TASKS; i++) 
	{
		int* x = new int();
		*x = i+1;
		CTask* t = new CTask(&hello, (void*) x);
#if 0
		bool loop=false;
		do 
		{
			if(!tp.AddTask(t))
			{
				SLEEP(10);
				loop=true;
			}

		} while (loop);
#else
		tp.WaitAddTask(t);
#endif 		
		cout << "Add to pool, task " << i+1 << endl;
	}

	tp.WaitAll();

	cout << "Exiting Application..." << endl;

	return 0;
}
