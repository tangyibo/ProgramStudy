#ifndef MTHREAD_H
#define MTHREAD_H
#include <pthread.h>

template<class T>
class ThreadHelper
{  
    friend class TheThread;
public:
    static pthread_t thread_create( T *obj, void (T::*mf)() )
    {
        ThreadHelper<T> *p=new ThreadHelper<T>(obj,mf);
        pthread_t thread_id;
        
        pthread_create(&thread_id,NULL,&(ThreadHelper<T>::thread_function),(void *)p);
        return thread_id;
    }
    
    ThreadHelper( T *obj, void  (T::*mf)())
    :obj_(obj),mf_(mf)
    {}
    
protected:
    
    static void * thread_function(void *param)
    {
        ThreadHelper<T> *p=(ThreadHelper<T> *)param;
        T* obj=p->obj_;
        void  (T::*mf)()=p->mf_;
        
        delete p;
        ( obj->*mf )();

        return 0;
    }
    
    T *obj_;
    void  (T::*mf_)();
};

template<class T>
static pthread_t create_thread ( T *obj, void (T::*mf )( ) )
{
    return ThreadHelper<T>::thread_create ( obj, mf );
}

/*
example:
#include <iostream>
#include "mthread.h"
using namespace std;

class test
{
public:
    test() {}

    void func()
    {
        cout << "enter test::func()" << endl;
        sleep(2);
        cout << "leave test::func()" << endl;
    }
};

int main(int argc, char** argv)
{
    test t;
    create_thread(&t,&test::func);
    while(1);
   return 0;
}

 */


#endif /* MTHREAD_H */

