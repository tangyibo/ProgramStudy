#include <iostream>
#include <boost/smart_ptr.hpp>
#include "MysqlConnPool.h"

const uint32_t port = 3306;
const std::string ipaddr = "172.16.90.119";
const std::string username = "tangyibo";
const std::string password = "tangyibo";
const std::string dbname = "dlut_data_sync";

int test()
{
    boost::shared_ptr<MySQLdb> connPtr;
    connPtr.reset(new MySQLdb());
    connPtr->Init(ipaddr, username, password, dbname, port);

    std::string sql = " SELECT * FROM `tmp_all_user_organization` ";
    std::vector<std::map<std::string, MySQLdb::dbvalue> > result;
    if (0 != connPtr->Query(sql.c_str(), result))
    {
        std::cout << "error:" << connPtr->GetErrorMessage() << std::endl;
        return 1;
    }

    std::vector<std::map<std::string, MySQLdb::dbvalue> >::const_iterator it;
    for (it = result.begin(); it != result.end(); ++it)
    {
        std::map<std::string, MySQLdb::dbvalue> entries = *it;
        std::map<std::string, MySQLdb::dbvalue>::iterator item;
        for (item = entries.begin(); item != entries.end(); ++item)
        {
            std::cout << item->first << ":" << item->second.to_string() << "|";
        }
        std::cout << std::endl;
    }
    
    return 0;
}

int test2()
{  
    DBConnHelper<MySQLdb,MysqlConnectorPool> db_helper;
    boost::shared_ptr<MySQLdb> connPtr = db_helper.get();

    std::string sql = " INSERT INTO `dlut_data_sync`.`db_status` (`table_name`, `num`, `sync_time`) VALUES ('test', '991', '2018-07-27 11:16:07'); ";
    int affected_rows=0;
    if(0==connPtr->Execute(sql.c_str(),&affected_rows))
    {
        std::cout <<"Execute SQL affected rows:"<<affected_rows<< std::endl;
    }
    else
    {
         std::cout <<"Execute SQL failed,error:"<<connPtr->GetErrorMessage()<< std::endl;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
#if 0
    return test();
#else
    MysqlConnectorPool* pool = MysqlConnectorPool::getInstance();
    pool->init(ipaddr, username, password, dbname, port, 2);
    return test2();
#endif
}