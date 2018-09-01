#ifndef  _SRC_THREAD_POOL_H_
#define _SRC_THREAD_POOL_H_
#include <vector>
#include <deque>
#include "platform.h"

class CTask
{
public:
        CTask(void (*pFunc)(void*), void* arg);
        ~CTask(void);

        void Run();

private:
        void (*m_pFunc)(void*);
        void* m_arg;
};


class CMyThreadPool
{
public:
	CMyThreadPool();
	~CMyThreadPool();

	enum
	{
		DEFAULT_POOL_SIZE=10,
		DEFAULT_TASK_QUEUE_SIZE=50,

		TPS_STARTED=1,//thread pool state - started
		TPS_STOPPED=2,//thread pool state - stopped
	};

	int Initialize(const int pool_size=DEFAULT_POOL_SIZE,const int queue_size=DEFAULT_TASK_QUEUE_SIZE);
	int Destroy();

	bool AddTask(CTask *task);
	void WaitAddTask(CTask *task);
	void WaitAll();

public:
	void ExecThreadFunction();

private:
	bool CheckHasTask();
	unsigned int TaskQueueSize();

private:
	unsigned int   m_nPoolSize;
	unsigned int   m_nQueueSize;
	std::vector<THREAD_TYPE>   m_threads; 
	std::deque<CTask*>   m_queueTasks;
	CMutex  m_mutexTasks;//m_queueTasks与m_statePool的互斥信号量
	CEvent   m_condTasks;//条件信号量

	volatile int   m_statePool;
};

#endif
