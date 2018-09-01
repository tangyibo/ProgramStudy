#ifndef _MYSQLDB_WRAPPER_HEADER_H_
#define _MYSQLDB_WRAPPER_HEADER_H_
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <stdint.h>
#include "mysql.h"

//MySQL重连接的次数
#define MYSQL_CONNECT_RETRY_COUNT      3

class MySQLdb
{
public:

    class dbvalue
    {
        friend class MySQLdb;
    public:

        dbvalue ( )
        {
            data_ = "";
            is_null_ = true;
        }

        virtual ~dbvalue ( )
        {
        }

        int size ( )
        {
            if ( is_null_ )
            {
                return -1;
            }
            return data_.size ( ) - 1;
        }

        const char* data ( )
        {
            if ( is_null_ )
            {
                return NULL;
            }
            return data_.data ( );
        }

        const std::string to_string ( )
        {
            if ( is_null_ )
            {
                return "";
            }
            return data_;
        }

        template <typename T>
        T as ( )
        {
            T asVal;
            pri_as ( asVal );

            return asVal;
        }

    private:

        int set ( const char* buf, int len )
        {
            if ( !buf )
            {
                is_null_ = true;
                return 0;
            }

            is_null_ = false;
            data_.assign ( buf, len );

            return 0;
        }

        int pri_as ( char *& val )
        {
            val = ( char* ) data ( );
            return 0;
        }

        int pri_as ( const char *& val )
        {
            val = data ( );
            return 0;
        }

        int pri_as ( char& val )
        {
            int32_t tmp_val;
            int ret = pri_as ( tmp_val );
            val = tmp_val;
            return ret;
        }

        int pri_as ( unsigned char& val )
        {
            uint32_t tmp_val;
            int ret = pri_as ( tmp_val );
            val = tmp_val;
            return ret;
        }

        int pri_as ( std::string& val )
        {
            if ( is_null_ )
            {
                val = "";
            }
            val = data_.data ( );
            return 0;
        }

        template <typename T>
        int pri_as ( T& val )
        {

            if ( this->size ( ) <= 0 )
            {
                val = 0;
                return 0;
            }

            std::stringstream ss;
            ss << this->data ( );
            ss >> val;
            return 0;
        }

    private:
        std::string data_;
        bool is_null_;
    };

public:
    /*
     * 构造函数
     */
    MySQLdb ( );
    
    /*
     * 析构函数
     */
    virtual ~MySQLdb ( );

    /*
     * 获取最后一次操作的错误信息
     * @return 错误信息
     */
    const char *GetErrorMessage ( )
    {
        return errmsg_;
    }

    /*
     * 初始化参数配置
     * @param ip,  string， 数据库的连接IP地址
     * @param user, string ,  连接数据库的用户名
     * @param pwd, string ,  连接数据库的密码
     * @param dbname, string ,  连接的数据库名称
     * @param port, int ,  数据库的连接端口
     * @param charset, string ,  字符编码集
     */
    void Init ( const std::string &ip, const std::string &user, const std::string &pwd, const std::string &dbname, const uint32_t &port = 3306, const std::string &charset = "" );

    /*
     * 建立与数据库的连接
     * @return 成功返回0，非0为失败，通过GetErrorMessage ( )获取失败信息
     */
    int Open ( );

    /*
     * 关闭与数据库的连接
     */
    void Close ( );
    
    /*
     * 是否已经与数据库建立连接
     */
    bool IsConnected ( )
    {
        if ( !opened_ || NULL == mysqldb_ )
            return false;

        return true;
    }

    /*
     * 执行insert/update/delete等修改数据的操作
     * @param sql, string ,  要执行的SQL语句
     * @param affected_rows, int* ,  接收返回受影响的行数
     * @return 成功返回0，非0为失败，通过GetErrorMessage ( )获取失败信息
     */
    int Execute ( const char* sql, int* affected_rows = NULL );

     /*
     * 执行select查询数据的操作
     * @param sql, string ,  要执行的SQL语句
     * @param list, std::vector<std::map<std::string, MySQLdb::dbvalue> > ,  查询结果数据集
     * @return 成功返回0，非0为失败，通过GetErrorMessage ( )获取失败信息
     */
    int Query ( const char* sql, std::vector<std::map<std::string, MySQLdb::dbvalue> > &list );

     /*
     * 转义SQL语句中使用的字符串中的特殊字符
     * @param src, char* ,  字符串的起始地址
     * @param len, int, 字符串的长度
     * @return 成功返回转义后的字符串，失败返回空字符串""
     */
    std::string EscStr ( const char* src, uint32_t len );

protected:
    
    /*
     * 检查与数据库的连接
     * @return 连接成功返回true，false为失败，通过GetErrorMessage ( )获取失败信息
     */
    bool CheckConnected ( );

private:
    char errmsg_[1024];
    MYSQL* mysqldb_;
    bool opened_;

    uint32_t port_;
    std::string ipaddr_;
    std::string username_;
    std::string password_;
    std::string dbname_;
    std::string charset_;
};

#endif /* _MYSQLDB_WRAPPER_HEADER_H_ */

