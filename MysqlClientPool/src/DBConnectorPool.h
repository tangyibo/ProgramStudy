#ifndef _DB_CONN_HELPER_H_
#define _DB_CONN_HELPER_H_
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

/*
 * 数据库连接池基类
 */
template< class Connector >
class DBConnectorPool: boost::noncopyable
{
public:
    /*
     * 析构函数
     */
    virtual ~DBConnectorPool ( ){}
    
    /*
     * 从连接池中获取一条连接
     */
    virtual boost::shared_ptr<Connector> getConnect ( ) = 0;

    /*
     * 归还一条连接到连接池中
     */
    virtual void retConnect ( boost::shared_ptr<Connector> &conn ) = 0;
};

/*
 * MySQL/Redis连接帮助类，用于在栈上获取MySQL/Redis连接池中
 * 获取连接器，并进行自动回收管理
 */
template< class Connector,class DBConnectorPool >
class DBConnHelper
{
public:

    DBConnHelper ( )
    {
        connPoolPtr = DBConnectorPool::getInstance ( );
    }

    boost::shared_ptr< Connector > get ( )
    {
        if ( !this->dbConnPtr.get ( ) )
        {
            this->dbConnPtr = connPoolPtr->getConnect ( );
        }

        return this->dbConnPtr;
    }

    ~DBConnHelper ( )
    {
        connPoolPtr->retConnect ( this->dbConnPtr );
    }

private:
    DBConnectorPool* connPoolPtr;
    boost::shared_ptr< Connector > dbConnPtr;
};

#endif
