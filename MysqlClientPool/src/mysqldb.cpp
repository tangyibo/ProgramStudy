#include "mysqldb.h"
#include <stdio.h>
#include <string.h>

#define MYSQL_WRAPPER_ERROR_MESSAGE(buffer,fmt, args...)                          \
    snprintf(buffer, sizeof(buffer), "[%s][%d][%s]"fmt, __FILE__, __LINE__,__FUNCTION__, ##args)

class auto_recycle_result
{
public:
    auto_recycle_result(MYSQL_RES* result)
    {
        result_ = result;
    }
    
    ~auto_recycle_result()
    {
        if(NULL != result_)
        {
            mysql_free_result(result_);
            result_ = NULL;
        }
    }

private:
    MYSQL_RES *result_;
};

////////////////////////////////////////////////////

MySQLdb::MySQLdb()
:mysqldb_(NULL)
,opened_(false)
,port_(3306)
,ipaddr_("")
,username_("")
,password_("")
,dbname_("")
,charset_("")
{
    memset(errmsg_,0,sizeof(errmsg_));
}

MySQLdb::~MySQLdb()
{
    if(opened_)
    {
        Close();
    }
}

void MySQLdb::Init ( const std::string &ip, const std::string &user, const std::string &pwd, const std::string &dbname, const uint32_t &port, const std::string &charset )
{
    port_ = port;
    ipaddr_ = ip;
    username_ = user;
    password_ = pwd;
    dbname_ = dbname;
    charset_ = charset;
}

int MySQLdb::Open()
{
    Close();
    
    mysqldb_=mysql_init(NULL);
    if(NULL==mysqldb_)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error: Unable to initialize MySQL API");
        return -1;
    }
    
    if (!charset_.empty())
    {
        if (mysql_options(mysqldb_, MYSQL_SET_CHARSET_NAME, charset_.c_str()) != 0)
        {
            MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_, "mysql_options MYSQL_SET_CHARSET_NAME Error [%u]: %s",mysql_errno(mysqldb_), mysql_error(mysqldb_));
            return -1;
        }
    }

    int connect_timeout=10;
    int read_timeout=10;
    int write_timeout=10;
    mysql_options(mysqldb_, MYSQL_OPT_CONNECT_TIMEOUT, (const char *) &connect_timeout);
    mysql_options(mysqldb_, MYSQL_OPT_READ_TIMEOUT, (const char *) &read_timeout);
    mysql_options(mysqldb_, MYSQL_OPT_WRITE_TIMEOUT, (const char *) &write_timeout);

    char reconnect = 1;
    if (mysql_options(mysqldb_, MYSQL_OPT_RECONNECT, &reconnect) != 0)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"mysql_options MYSQL_OPT_RECONNECT Error [%u]: %s", mysql_errno(mysqldb_), mysql_error(mysqldb_));
        return -1;
    }
    
     if(!mysql_real_connect(mysqldb_, ipaddr_.c_str(), username_.c_str(), password_.c_str(), dbname_.c_str(), port_, NULL, 0))
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error: Unable to connect to server[%s]",mysql_error(mysqldb_));
        return -1;
    }
    
    opened_=true;
    return 0;
}

void MySQLdb::Close()
{
    opened_=false;
    if(mysqldb_)
    {
        mysql_close(mysqldb_);
        mysqldb_ = NULL;
    }
}

bool MySQLdb::CheckConnected()
{
    int retry_count = MYSQL_CONNECT_RETRY_COUNT;
    if (!opened_ || NULL == mysqldb_)
    {
        while (retry_count > 0)
        {
            if (!Open())
                break;
            
            retry_count--;
        }
        
        if (retry_count == 0)
            return false;
    }
    
    return true;
}

int MySQLdb::Execute(const char* sql,int* affected_rows)
{
    if(!CheckConnected())
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error: Unable to connect to server[%s]",mysql_error(mysqldb_));
        return -1;
    }
    
    if(mysql_query(mysqldb_, sql) != 0)
    {
        unsigned int error_code= mysql_errno(mysqldb_);
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error: Unable to query,errMsg=%s,errCode=%u", mysql_error(mysqldb_), error_code);
        if(error_code==2006u||error_code==2013u)
        {
                 return Execute(sql);
        }
        
        return -1;
    }

    if (affected_rows)
    {
        int ret = mysql_affected_rows(mysqldb_);
        if (ret >= 0)
        {
            *affected_rows = ret;
            return 0;
        }
        
        return ret;
    }
    
    return 0;
}

int MySQLdb::Query(const char* sql, std::vector<std::map<std::string, dbvalue> > &list)
{
    list.clear();
    int ret = Execute(sql, NULL);
    if(0!=ret)
    {
        return ret;
    }

    MYSQL_RES * result = mysql_store_result(mysqldb_);
    if (!result)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_, "Error: Unable to retrieve result");
        return -1;
    }
    
    auto_recycle_result  recycle(result);
    
    uint32_t unRecords = mysql_num_rows(result);
    if (0 == unRecords)
    {
        return 0;
    }

    uint32_t num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    if (!fields)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error: mysql_fetch_fields() fail");
        return -1;
    }

    unsigned long *len = NULL;
    MYSQL_ROW row;
    for (uint32_t unIndex = 0; unIndex < unRecords; unIndex++)
    {
        row = mysql_fetch_row(result);
        len = mysql_fetch_lengths(result);
        if (!len)
        {
            MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error mysql_fetch_lengths fail,index:%u", unIndex);
            return -1;
        }

        std::map<std::string, MySQLdb::dbvalue> tmp;
        for (uint32_t i = 0; i < num_fields; i++)
        {
            MySQLdb::dbvalue &value = tmp[fields[i].name];
            value.set((char*) row[i], len[i]);
        }
        
        list.push_back(tmp);
    }

    return 0;
}

std::string MySQLdb::EscStr(const char* src, uint32_t len)
{
    if(!mysqldb_)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error: query error,m_Database is null\n");
        return "";
    }

    if (!src)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_,"Error:EscStr() input param src is null");
        return "";
    }

    char *escapeBuff = new (std::nothrow) char[len * 2 + 1];
    if (NULL == escapeBuff)
    {
        MYSQL_WRAPPER_ERROR_MESSAGE(errmsg_, "Error:memory allocate failed");
        return "";
    }
    
    mysql_real_escape_string(mysqldb_,escapeBuff,src,len);
    std::string strDest(escapeBuff);

    delete [] escapeBuff;
    escapeBuff = NULL;

    return strDest;
}