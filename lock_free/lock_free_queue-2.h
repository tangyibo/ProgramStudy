#ifndef LOCK_FREE_TASK_QUEUE_H
#define LOCK_FREE_TASK_QUEUE_H
#include <boost/lockfree/queue.hpp>
#include <unistd.h>

template <typename T>
class lock_free_task_queue
{
public:
    lock_free_task_queue():cur_size(0){}
    ~lock_free_task_queue(){}
    
    void push ( const T& t )
    {
        while ( true )
        {
            if ( !tasks.push ( t ) )
                usleep(10);
            else
            {
                __sync_fetch_and_add(&cur_size,1);
                break;
            }
        }
    }

    bool pop_nonblock ( T &t )
    {
        bool ret= tasks.pop ( t );
        if(ret)
        {
            __sync_fetch_and_sub(&cur_size,1);
        }
        return ret;
    }

    bool pop_nonblock ( std::list<T> &l, const size_t count = 0 )
    {
        l.clear ( );
        
        T t;
        while(tasks.pop ( t ))
        {
            __sync_fetch_and_sub(&cur_size,1);
            l.push_back (t);

            if ( count > 0 && l.size ( ) >= count )
                break;
        }

        return (l.size ( ) > 0 );
    }

    T pop_block ( )
    {
        T t;
        while(!tasks.pop ( t ))
            usleep(10);

        __sync_fetch_and_sub(&cur_size,1);
        return t;
    }

    const int size ( )
    {
        return cur_size;
    }

private:
    boost::lockfree::queue< T, boost::lockfree::capacity<4096> > tasks;
    unsigned int cur_size;
};  


#endif /* LOCK_FREE_TASK_QUEUE_H */

