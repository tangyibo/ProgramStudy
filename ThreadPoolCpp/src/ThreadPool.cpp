#include "ThreadPool.h"
#include <iostream>
#include <assert.h>

extern "C" void THREAD_API MyThreadFunc(void* arg)
{
	CMyThreadPool* tp = (CMyThreadPool*) arg;
	tp->ExecThreadFunction();
	
	END_THREAD();
	THREAD_EXIT;
}

//////////////////////////////////////////////
CTask::CTask(void (*pFunc)(void*), void* arg)
:m_pFunc(pFunc)
,m_arg(arg)
{
}

CTask::~CTask(void)
{

//        if (m_arg != 0)  
//        {   
//                delete m_arg;
//                m_arg=0;
//        }   
}

void CTask::Run()
{
        if(m_pFunc)
        {
                (*m_pFunc)(m_arg);
        }
        else
        {
		assert(0);
	}
}

//////////////////////////////////////////////
CMyThreadPool::CMyThreadPool()
:m_nPoolSize(DEFAULT_POOL_SIZE)
,m_nQueueSize(DEFAULT_TASK_QUEUE_SIZE)
,m_mutexTasks()
,m_condTasks()
,m_statePool(TPS_STOPPED)
{
	m_threads.clear();
}

CMyThreadPool::~CMyThreadPool()
{
	if(TPS_STOPPED!=m_statePool)
		Destroy();
}

int CMyThreadPool::Initialize(const int pool_size,const int queue_size)
{
	if(pool_size<1 || queue_size <1)
	{
		std::cerr<<"pool_size<1 or  queue_size <1"<<std::endl;
		return -1;
	}

	m_nPoolSize=pool_size;
	m_nQueueSize=queue_size;
	m_statePool=TPS_STARTED;

	int ok_thread_count=0;
	for(unsigned int i=0;i<m_nPoolSize;++i)
	{
		DEFINE_THREAD(oneThread);
		BEGIN_THREAD(oneThread,MyThreadFunc,this);
		if(FAILED_THREAD(oneThread))
		{
			std::cerr<<"create thread failed()!"<<oneThread<<std::endl;
		}
		else
		{
			ok_thread_count++;
			m_threads.push_back(oneThread);
		}
	}

	return ok_thread_count-m_nPoolSize;
}

int CMyThreadPool::Destroy()
{
	if(TPS_STOPPED==m_statePool)
		return 0;

	{
		CLock<CMutex> g(m_mutexTasks);
		m_statePool=TPS_STOPPED;
	}

	//std::cout << "Broadcasting STOP signal to all threads..." << std::endl;
	m_condTasks.broadcast();

	for(unsigned int i=0;i<m_nPoolSize;++i)
	{
		m_condTasks.broadcast();
		WAITTHREADEND(m_threads[i]);
	}

	return 0;
}

bool CMyThreadPool::CheckHasTask()
{
#ifdef WIN32
	CLock<CMutex> g(m_mutexTasks);
#endif
	return  (TPS_STOPPED!=m_statePool) && (m_queueTasks.empty());
}

void CMyThreadPool::ExecThreadFunction()
{
	CTask *task=NULL;

	while(1)
	{
#ifndef WIN32
		m_mutexTasks.lock();
#endif
		while ( CheckHasTask()) 
		{
			// Wait until there is a task in the queue
			// In Linux  function pthread_cond_wait() can unlock mutex while wait, then lock it
			// back when signaled , but in windows it is not, so we add macros  to simulate this
			// condition.
			m_condTasks.wait(10,m_mutexTasks);
		}

#ifdef WIN32
		m_mutexTasks.lock();
#endif

		if(TPS_STOPPED==m_statePool)
		{
			m_mutexTasks.unlock();
			break;
		}

		if(!m_queueTasks.empty())
		{
			task=m_queueTasks.front();
			m_queueTasks.pop_front();
		}
	
		m_mutexTasks.unlock();

		if(task)
		{
			task->Run();
			delete task;
			task=NULL;
		}

	}
}

bool CMyThreadPool::AddTask(CTask *task)
{
	size_t curr_size=TaskQueueSize();
	if(curr_size>=m_nQueueSize)
		return false;
	
	CLock<CMutex> g(m_mutexTasks);
	m_queueTasks.push_back(task);
	m_condTasks.signal();
	return true;
}

void CMyThreadPool::WaitAddTask(CTask *task)
{
        bool loop=false;
        do
        {
                size_t queue_size=TaskQueueSize();
                if(queue_size>=m_nQueueSize)
                {
                        usleep(100);
                        loop=true;

                }
                else
                {
                        loop=!AddTask(task);
                }
        } while (loop);
}

unsigned int  CMyThreadPool::TaskQueueSize()
{
	CLock<CMutex> g(m_mutexTasks);
	return m_queueTasks.size();
}


void CMyThreadPool::WaitAll()
{
#if 0
	while(0!=TaskQueueSize())
		SLEEP(10);
#else
        int trycount=10;

        int counter=trycount;
        do{
                if(0!=TaskQueueSize())
                {
                        counter=trycount;
                        SLEEP(10);
                }
                else
                {
                        --counter;
                }
        }while(counter>0);

#endif

	Destroy();
}
