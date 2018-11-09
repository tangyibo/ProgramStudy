#!/usr/bin/python
# -*- coding: UTF-8 -*-
# Date: 2018-10-24
# Author: tang
#

import sys, os
import MySQLdb
import ConfigParser
import logging
import logging.handlers

# 加载日志配置
real_dir = os.path.dirname(os.path.realpath(__file__))
prog_name = os.path.basename(sys.argv[0])
name, suffix = os.path.splitext(prog_name)
os.system("mkdir -p %s/log" % real_dir)
logfilename = "%s/log/log-%s.log" % (real_dir, name)
handler = logging.handlers.RotatingFileHandler(logfilename, maxBytes=1024 * 1024, backupCount=10)
formatter = logging.Formatter('%(asctime)s - [%(levelname)-8s]  - %(message)s  %(filename)s:%(lineno)s')
handler.setFormatter(formatter)
logger = logging.getLogger(logfilename)
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)


class MYSLQTYPE:
    NULL = 0  # no mysql
    SRC = 1  # source mysql
    DEST = 2  # destination mysql
    ALL = 3  # source/destination mysql


class ConfigFile:

    def __init__(self):
        self.__cf = ConfigParser.ConfigParser()

    def parse(self, filename):
        if not os.path.exists:
            raise RuntimeError('file not exist:%s' % filename)

        self.__cf.read(filename)

        self.source_mysql_host = self.__cf.get("source", "host")
        self.source_mysql_port = int(self.__cf.get("source", "port"))
        self.source_mysql_user = self.__cf.get("source", "user")
        self.source_mysql_passwd = self.__cf.get("source", "passwd")
        self.source_mysql_dbname = self.__cf.get("source", "dbname")

        self.destination_mysql_host = self.__cf.get("destination", "host")
        self.destination_mysql_port = int(self.__cf.get("destination", "port"))
        self.destination_mysql_user = self.__cf.get("destination", "user")
        self.destination_mysql_passwd = self.__cf.get("destination", "passwd")
        self.destination_mysql_dbname = self.__cf.get("destination", "dbname")

        source_mysql_tables_list = self.__cf.get("source", "tbname").split(",")
        destination_mysql_tables_list = self.__cf.get("destination", "tbname").split(",")

        src_len = len(source_mysql_tables_list)
        dest_len = len(destination_mysql_tables_list)
        if src_len != dest_len:
            raise RuntimeError('source table list count(%d) not equal destination table list count(%d)',
                               (src_len, dest_len))

        self.mysql_table_map = dict(zip(source_mysql_tables_list, destination_mysql_tables_list))


class MySQL2MySQL:

    def __init__(self, file_name):
        self.config = ConfigFile()
        self.config.parse(file_name)

    def connect(self, server=MYSLQTYPE.ALL):

        if server & MYSLQTYPE.SRC > 0:
            # 建立与源端数据库的连接
            self.source_conn = MySQLdb.connect(
                host=self.config.source_mysql_host,
                user=self.config.source_mysql_user,
                passwd=self.config.source_mysql_passwd,
                db=self.config.source_mysql_dbname,
                port=self.config.source_mysql_port,
                charset='utf8')

        if server & MYSLQTYPE.DEST > 0:
            # 建立与目的端数据库的连接
            self.destination_conn = MySQLdb.connect(
                host=self.config.destination_mysql_host,
                user=self.config.destination_mysql_user,
                passwd=self.config.destination_mysql_passwd,
                db=self.config.destination_mysql_dbname,
                port=self.config.destination_mysql_port,
                charset='utf8')

            # 该选项影响列为自增长的插入。在默认设置下，插入0或者null代表生成下一个自
            # 增长值。如果用户希望插入的值为0，而该列又是自增长的
            dest_cursor = self.destination_conn.cursor()
            dest_cursor.execute("SET sql_mode='NO_AUTO_VALUE_ON_ZERO';")

    def run(self):
        self.connect()
        success = True
        for src_table in self.config.mysql_table_map:
            if not self.handle_one_table(src_table, self.config.mysql_table_map[src_table]):
                success = False

        return success

    def handle_one_table(self, source_table, destination_table):
        src_cursor = self.source_conn.cursor()
        dest_cursor = self.destination_conn.cursor()

        logger.info("handle table :%s=>%s" % (source_table, destination_table))

        # 获取建表结构
        show_create_table_sql = "show create table %s; " % source_table
        try:
            src_cursor.execute(show_create_table_sql)
        except MySQLdb.OperationalError, e:
            logger.info("reconnect destination mysql...")
            self.connect(MYSLQTYPE.SRC)
            src_cursor = self.source_conn.cursor()
            src_cursor.execute(show_create_table_sql)
        except Exception, e:
            logger.error("execute show create sql for table (%s)failed:%s" % (source_table, show_create_table_sql))
            return False

        results = src_cursor.fetchone()
        drop_table_sql = "DROP TABLE IF EXISTS `%s`;" % destination_table
        create_table_sql = results[1].replace(source_table, destination_table)

        success = True
        try:
            dest_cursor.execute(drop_table_sql)
            dest_cursor.execute(create_table_sql)
            self.destination_conn.commit()
        except MySQLdb.OperationalError, e:
            logger.info("reconnect destination mysql...")
            self.connect(MYSLQTYPE.DEST)
            dest_cursor = self.destination_conn.cursor()
            dest_cursor.execute(drop_table_sql)
            dest_cursor.execute(create_table_sql)
            self.destination_conn.commit()
        except Exception, e:
            self.destination_conn.rollback()
            logger.error("execute create sql for table (%s)failed:%s" % (destination_table, e))
            return False

        # 表数据复制
        query_table_sql = "select * from %s" % source_table

        try:
            src_cursor.execute(query_table_sql)
        except MySQLdb.OperationalError, e:
            logger.info("reconnect source mysql...")
            self.connect(MYSLQTYPE.SRC)
            src_cursor = self.source_conn.cursor()
            src_cursor.execute(query_table_sql)
        except Exception, e:
            self.destination_conn.rollback()
            logger.error("execute query sql failed:%s" % (query_table_sql, e))
            return False

        while True:
            row = src_cursor.fetchone()
            if row is None:
                break
            else:
                if not self.data_insert(dest_cursor, destination_table, row):
                    success = False

        src_cursor.close()
        dest_cursor.close()
        return success

    def data_insert(self, cursor, table, row):
        data = "("
        for col in row:
            if col is None:
                data += """NULL"""
            else:
                data += """'%s'""" % MySQLdb.escape_string(str(col))
            data += ','
        data = data[:-1]
        data += ')'

        sql_statement = """insert into %s values %s""" % (table, data)
        try:
            cursor.execute(sql_statement)
            self.destination_conn.commit()
            return True
        except MySQLdb.OperationalError, e:
            logger.info("reconnect destination mysql...")
            self.connect(MYSLQTYPE.DEST)
            cursor = self.destination_conn.cursor()
            return self.data_insert(cursor, table, row)
        except Exception, e:
            logger.error("insert into MySQL for table (%s) Failed <%s>" % (table, e))
        return False

    def fini(self):
        self.source_conn.close()
        self.destination_conn.close()


if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')

    logger.info("server start ...")
    file_name = '%s/config.ini' % os.path.dirname(os.path.realpath(__file__))
    repl = MySQL2MySQL(file_name)
    ret = repl.run()
    repl.fini()
    if not ret:
        logger.error("server run failed!")
        sys.exit(0)
    else:
        logger.info("server run success!")
        sys.exit(1)

    # shell 调用方法:
    #
    # python hello.py
    #
    # if [ $?==0 ];then
    #     exit
    # else
    #     python world.py
    # fi
