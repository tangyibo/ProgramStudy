#! /usr/bin/python
# -*-encoding:utf-8-*-
# Date: 2018-11-28
# Author: tang
#
import sys, os  # operation system

# need to set this, or Oracle won't retrieve utf8 chars properly
os.environ["NLS_LANG"] = 'AMERICAN_AMERICA.AL32UTF8'
import cx_Oracle  # oracle client
import MySQLdb  # mysql client

def get_oracle_table_metadata(cursor):
    table_metadata = []
    # "The description is a list of 7-item tuples where each tuple
    # consists of a column name, column type, display size, internal size,
    # precision, scale and whether null is possible."
    for column in cursor.description:
        table_metadata.append({
            'name': column[0],
            'type': column[1],
            'display_size': column[2],
            'internal_size': column[3],
            'precision': column[4],
            'scale': column[5],
            'nullable': column[6],
        })
    return table_metadata


def create_mysql_table(mysql, table, table_metadata):
    sql = "CREATE TABLE %s (" % (table,)
    column_definitions = []

    for column in table_metadata:
        # 'LINES' is a MySQL reserved keyword
        column_name = column['name']
        if column_name == "LINES":
            column_name = "NUM_LINES"

        if column['type'] == cx_Oracle.NUMBER:
            #column_type = "DECIMAL(%s, %s)" % (column['precision'], column['scale'])
            column_type = "DECIMAL(%s, %s)" % (10,2)
        elif column['type'] == cx_Oracle.STRING:
            if column['internal_size'] > 256:
                column_type = "TEXT"
            else:
                column_type = "VARCHAR(%s)" % (column['internal_size'],)
        elif column['type'] == cx_Oracle.DATETIME:
            column_type = "DATETIME"
        elif column['type'] == cx_Oracle.FIXED_CHAR:
            column_type = "CHAR(%s)" % (column['internal_size'],)
        else:
            raise Exception("No mapping for column type %s" % (column['type'],))

        if column['nullable'] == 1:
            nullable = "null"
        else:
            nullable = "not null"

        column_definitions.append("%s %s %s" % (column_name, column_type, nullable))

    sql += ",".join(column_definitions)
    sql += ")ENGINE=InnoDB DEFAULT CHARACTER SET = utf8;"

    print "Create table SQL:%s" % sql
    
    cursor = mysql.cursor()
    cursor.execute('DROP TABLE IF EXISTS `%s`' % table)
    cursor.execute(sql)


def migrate_data(oracle, mysql, table):
    oracle_cursor = oracle.cursor()

    # cursor.rowcount is supposed to contain # of records in query but
    # that doesn't seem to work for oracle, so we do SELECT count(*)
    # instead.
    sql_count="SELECT count(*) FROM %s" % table
    oracle_cursor.execute(sql_count)
    total_rows = oracle_cursor.fetchone()[0]
    print "table [%s] enties total count:%d" % (table,total_rows)

    oracle_cursor.execute("SELECT * FROM %s" % table)
    table_metadata = get_oracle_table_metadata(oracle_cursor)
    create_mysql_table(mysql, table, table_metadata)

    for x in xrange(total_rows):
        # TODO: should probably use fetchmany() and transactions to speed things up
        row = oracle_cursor.fetchone()

        column_names = []
        column_values = []
        index = 0
        for column in table_metadata:
            if column['name'] == 'LINES':
                column_names.append('NUM_LINES')
            else:
                column_names.append(column['name'])
            column_values.append(row[index])
            index += 1

        question_marks = ",".join(["%s" for i in range(len(column_names))])
        sql_insert = "INSERT INTO %s (%s) VALUES (%s)" % (table, ",".join(column_names), question_marks)
        mysql_cursor = mysql.cursor()
        mysql_cursor.execute(sql_insert, column_values)
	mysql.commit()


if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')

    try:
        # oracle connect string uri
        # format username/password@ip_address/database_name
        oracle_connect_uri = "view_xhdx/view_xhdx@172.16.10.51:1521/orcl"
        oracle = cx_Oracle.connect(oracle_connect_uri)
        mysql = MySQLdb.connect(
            host='127.0.0.1',
            user='root',
            passwd='',
            db='test',
            port=3306,
            charset='utf8')
        tables = ('VIEW_FM_CODE_PUB','VIEW_T_ORG_DEPT','VIEW_T_STF_BASE','VIEW_T_STF_POSITION')
    except AttributeError, e:
        print "error",e
        sys.exit(1)

    for table in tables:
        print "Doing %s" % (table,)
        migrate_data(oracle, mysql, table)

    print "over!"
