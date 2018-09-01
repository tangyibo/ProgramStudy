#include "MysqlConnPool.h"
#include "log4z.h"

MysqlConnectorPool* MysqlConnectorPool::_instance = NULL;

MysqlConnectorPool* MysqlConnectorPool::getInstance()
{
    static boost::mutex locker;
    boost::unique_lock<boost::mutex> guard(locker);
    
    if (_instance == NULL)
    {
        _instance = new MysqlConnectorPool();
    }
    
    return _instance;
}

/////////////////////////////////////////////////////////
MysqlConnectorPool::MysqlConnectorPool()
:ipaddr_("127.0.0.1")
,port_(3306)
,username_("")
,password_("")
,dbname_("")
,size_(0)
,lock_()
,conList_()
{
}

MysqlConnectorPool::~MysqlConnectorPool()
{
    destoryPool();
}

boost::shared_ptr<MySQLdb> MysqlConnectorPool::getConnect()
{
    boost::unique_lock<boost::mutex> guard(lock_);
    boost::shared_ptr<MySQLdb> sp = conList_.front();
    conList_.pop_front();
    if (conList_.size() == 0)
    {
        LOG_TRACE(LOG4Z_MAIN_LOGGER_ID, "MySQLdbConnectionPool size:" << conList_.size());
    }
    
    if (!sp->IsConnected())
    {
        sp->Open();
    }
    
    return sp;
}

boost::shared_ptr<MySQLdb> MysqlConnectorPool::connect()
{
    boost::shared_ptr<MySQLdb> tmp;
    tmp.reset(new MySQLdb());
    tmp->Init(ipaddr_, username_, password_, dbname_, port_,"utf8");
    
    LOG_DEBUG(LOG4Z_MAIN_LOGGER_ID, "MySQL server["<<conList_.size()<<"]:" << ipaddr_<<":"<<port_ << "account:" << username_<<"/" <<  password_);
    
    if (!tmp->Open())
    {
        LOG_ERROR(LOG4Z_MAIN_LOGGER_ID, "MySQL connect error!!");
    }
    
    return tmp;
}

int MysqlConnectorPool::getPoolSize()
{
    boost::unique_lock<boost::mutex> guard(lock_);
    return conList_.size();
}

void MysqlConnectorPool::addConn(size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        boost::shared_ptr<MySQLdb> conn = connect();
        conList_.push_back(conn);
    }
}

void MysqlConnectorPool::initConnectPool(size_t initialSize)
{
    boost::unique_lock<boost::mutex> guard(lock_);
    addConn(initialSize);
}

void MysqlConnectorPool::destoryOneConn()
{
    boost::shared_ptr<MySQLdb> sp = conList_.front();
    
    sp->Close();
    
    conList_.pop_front();
    
    --size_;
}

void MysqlConnectorPool::destoryPool()
{
    while (conList_.size())
    {
        boost::shared_ptr<MySQLdb> sp = conList_.front();
        sp->Close();
        conList_.pop_front();
    }
}

void MysqlConnectorPool::expandPool(size_t size)
{
    boost::unique_lock<boost::mutex> guard(lock_);
    addConn(size);
    size_ += size;
}

void MysqlConnectorPool::reducePool(size_t size)
{
    boost::unique_lock<boost::mutex> guard(lock_);
    if (size > size_)
    {
        return;
    }

    for (size_t i = 0; i < size; i++)
    {
        destoryOneConn();
    }
    size_ -= size;
}

void MysqlConnectorPool::retConnect(boost::shared_ptr<MySQLdb> &ret)
{
    boost::unique_lock<boost::mutex> guard(lock_);
    conList_.push_front(ret);
}

void MysqlConnectorPool::init(const std::string &ip, const std::string &user, const std::string &pwd, const std::string &dbname, const uint32_t &port ,const size_t &capacity )
{
    ipaddr_ = ip;
    port_ = port;
    username_ = user;
    password_ = pwd;
    dbname_ = dbname;
    
    size_ = capacity;
    initConnectPool(size_);
}