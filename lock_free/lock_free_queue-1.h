#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

/*
 * volatile实现无锁队列
 * 这个仅限于特定硬件
 */
template <typename T>  
class lock_free_queue
{
    struct qnode
    {
        struct qnode *next;
        T  data;
    };
    
public:

    queue ( )
    {
        init ( );
    }

    ~queue ( )
    {
        destroy ( );
    }

    bool init ( )
    {
        m_front = m_rear = new qnode<T>;
        if ( !m_front )
            return false;
        
        m_front->next = 0;
        
        return true;
    }

    void destroy ( )
    {
        while ( m_front )
        {
            m_rear = m_front->next;
            delete m_front;
            m_front = m_rear;
        }
    }

    bool push ( T &e )
    {
        struct qnode<T> *p = new qnode<T>;
        if ( !p )
            return false;
        
        p->next = 0;
        m_rear->next = p;
        m_rear->data = e;
        m_rear = p;
        
        return true;
    }

    bool pop ( T &e )
    {
        if ( m_front == m_rear )
            return false;

        struct qnode<T> *p = m_front;
        e = p->data;
        m_front = p->next;
        delete p;
        
        return true;
    }
    
private:
    struct qnode<T> * volatile m_front;
    struct qnode<T> * volatile m_rear;
};  

#endif /* LOCK_FREE_QUEUE_H */

