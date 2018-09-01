#ifndef _REDIS_CONNECTION_POOL_H_
#define _REDIS_CONNECTION_POOL_H_
#include <boost/thread/mutex.hpp>  
#include <boost/thread/locks.hpp>  
#include <string>
#include <list>
#include "DBConnectorPool.h"
#include "mysqldb.h"

/*
 * MySQLdb连接池
 */
class MysqlConnectorPool : public DBConnectorPool<MySQLdb>
{
public:
    /*
     * 析构函数
     */
    virtual ~MysqlConnectorPool ( );

    /*
     * 获取数据库连接池对象 static单例模式
     */
    static MysqlConnectorPool* getInstance ( );

    /*
     * 初始化连接器
     * @param connstr,  string， 连接字符串
     * @param capacity, size_t ,  初始化连接池的容量
     */
    void init ( const std::string &ip, const std::string &user, const std::string &pwd, const std::string &dbname, const uint32_t &port ,const size_t &capacity );

    /*
     * 从连接池中获取一条连接
     */
    virtual boost::shared_ptr<MySQLdb> getConnect ( );

    /*
     * 归还一条连接
     * @param ret，Connector，要归还的连接
     */
    virtual void retConnect ( boost::shared_ptr<MySQLdb> &ret );

    /*
     * 获取连接池的容量
     */
    int getPoolSize ( );

protected:
    boost::shared_ptr<MySQLdb> connect ( );

private:
    MysqlConnectorPool ( );

    //初始化连接池
    void initConnectPool ( size_t initialSize );

    //毁坏连接池
    void destoryPool ( );

    //销毁连接池中的一个连接
    void destoryOneConn ( );

    //扩大数据库连接池
    void expandPool ( size_t size );

    //缩小数据库连接池
    void reducePool ( size_t size );

    //增加size个连接
    void addConn ( size_t size );

    std::string ipaddr_;
    uint32_t port_;
    std::string username_;
    std::string password_;
    std::string dbname_;
    
    size_t size_; //pool size
    boost::mutex lock_; //锁
    std::list< boost::shared_ptr<MySQLdb> > conList_; //存放所有连接

    static MysqlConnectorPool *_instance; //连接池对象
};

#endif
