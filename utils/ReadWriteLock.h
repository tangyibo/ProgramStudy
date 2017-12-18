#ifndef _READ_WRITE_LOCK_H_
#define _READ_WRITE_LOCK_H_

enum _LOCK_TYPE
{
	LOCK_LEVEL_NONE,//none lock
	LOCK_LEVEL_READ,//read lock
	LOCK_LEVEL_WRITE,//write lock
};

#ifdef _WIN32
#include <windows.h>
/*Windows下读写锁，实现共享读，独占写*/
class CReadWriteLock
{
public:
	CReadWriteLock() 
	{
		m_currentLevel = LOCK_LEVEL_NONE;
		m_readCount    = 0;
		m_unlockEvent  = ::CreateEvent( NULL, TRUE, FALSE, NULL );
		m_accessMutex  = ::CreateMutex( NULL, FALSE, NULL );
		::InitializeCriticalSection( &m_csStateChange );
	}
	~CReadWriteLock() 
	{
		::DeleteCriticalSection( &m_csStateChange );
		if ( m_accessMutex ) ::CloseHandle( m_accessMutex );
		if ( m_unlockEvent ) ::CloseHandle( m_unlockEvent );
	}

	bool lock( int level) 
	{
		bool  bresult    = true;
		DWORD waitResult = 0;

		waitResult = ::WaitForSingleObject( m_accessMutex, INFINITE );
		if ( waitResult != WAIT_OBJECT_0 )  return false;

		if ( level == LOCK_LEVEL_READ && m_currentLevel != LOCK_LEVEL_WRITE )
		{
			::EnterCriticalSection( &m_csStateChange );
			m_currentLevel = level;
			m_readCount += 1;
			::ResetEvent( m_unlockEvent );
			::LeaveCriticalSection( &m_csStateChange );
		}
		else if ( level == LOCK_LEVEL_READ && m_currentLevel == LOCK_LEVEL_WRITE )
		{
			waitResult = ::WaitForSingleObject( m_unlockEvent, INFINITE );
			if ( waitResult == WAIT_OBJECT_0 )
			{
				::EnterCriticalSection( &m_csStateChange );
				m_currentLevel = level;
				m_readCount += 1;
				::ResetEvent( m_unlockEvent );
				::LeaveCriticalSection( &m_csStateChange );
			}
			else bresult = false;
		}
		else if ( level == LOCK_LEVEL_WRITE && m_currentLevel == LOCK_LEVEL_NONE )
		{
			::EnterCriticalSection( &m_csStateChange );
			m_currentLevel = level;
			::ResetEvent( m_unlockEvent );
			::LeaveCriticalSection( &m_csStateChange );
		}
		else if ( level == LOCK_LEVEL_WRITE && m_currentLevel != LOCK_LEVEL_NONE )
		{
			waitResult = ::WaitForSingleObject( m_unlockEvent, INFINITE );
			if ( waitResult == WAIT_OBJECT_0 )
			{
				::EnterCriticalSection( &m_csStateChange );
				m_currentLevel = level;
				::ResetEvent( m_unlockEvent );
				::LeaveCriticalSection( &m_csStateChange );
			}
			else bresult = false;
		}

		::ReleaseMutex( m_accessMutex );
		return bresult;

	} // lock()

	bool unlock() 
	{ 
		::EnterCriticalSection( &m_csStateChange );
		if ( m_currentLevel == LOCK_LEVEL_READ )
		{
			m_readCount --;
			if ( m_readCount == 0 ) 
			{
				m_currentLevel = LOCK_LEVEL_NONE;
				::SetEvent (m_unlockEvent);
			}
		}
		else if ( m_currentLevel == LOCK_LEVEL_WRITE )
		{
			m_currentLevel = LOCK_LEVEL_NONE;
			::SetEvent ( m_unlockEvent );
		}
		::LeaveCriticalSection( &m_csStateChange );

		return true;
	}

public:
	int    m_currentLevel;
	int    m_readCount;   
	HANDLE m_unlockEvent; 
	HANDLE m_accessMutex;
	CRITICAL_SECTION m_csStateChange;
};  
#else
#include <pthread.h>

/*Linux下读写锁，实现共享读，独占写*/
class CReadWriteLock
{
public:
	CReadWriteLock() 
	{
		pthread_rwlock_init(&m_rwlock,NULL);
	}

	~CReadWriteLock() 
	{
		pthread_rwlock_destroy(&m_rwlock);
	}

	bool lock( int level) 
	{
		int ret=0;
		if(LOCK_LEVEL_READ==level)
			ret=pthread_rwlock_rdlock(&m_rwlock);
		else if(LOCK_LEVEL_WRITE==level)
			ret=pthread_rwlock_wrlock(&m_rwlock);
		else
			return false;

		return 0==ret;
	}

	bool unlock() 
	{
		return 0==pthread_rwlock_unlock(&m_rwlock);
	}

private:
	pthread_rwlock_t m_rwlock;
};

#endif /*_WIN32*/

template<class T>
class CRWLock
{
public:
	explicit CRWLock(T &lock,bool bWrite=false)
		:m_rwlock(lock)
	{
		if(bWrite)
			m_rwlock.lock(LOCK_LEVEL_WRITE);
		else
			m_rwlock.lock(LOCK_LEVEL_READ);
	}

	~CRWLock()
	{
		m_rwlock.unlock();
	}

private:
	T &m_rwlock;
};

#endif /*_READ_WRITE_LOCK_H_*/