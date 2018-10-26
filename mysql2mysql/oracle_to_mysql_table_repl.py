#! /usr/bin/python
# -*-encoding:utf-8-*-
# Date: 2018-05-29
# Author: tang
#
import sys, os  # operation system
import logging, logging.handlers  # logger
import cx_Oracle  # oracle client
import MySQLdb

# oracle connect string uri
# format username/password@ip_address/database_name
oracle_connect_uri = "view_xhdx/view_xhdx@172.16.10.51:1521/orcl"

real_dir = os.path.dirname(os.path.realpath(__file__))
prog_name = os.path.basename(sys.argv[0])
name, suffix = os.path.splitext(prog_name)
os.system("mkdir -p %s/log" % real_dir)
logfilename = "%s/log/log-%s.log" % (real_dir, name)
handler = logging.handlers.RotatingFileHandler(logfilename, maxBytes=1024 * 1024, backupCount=5)
formatter = logging.Formatter('%(asctime)s - [%(levelname)-8s]  - %(message)s  %(filename)s:%(lineno)s')
handler.setFormatter(formatter)
logger = logging.getLogger(logfilename)
logger.addHandler(handler)
logger.setLevel(logging.DEBUG)


class OracleConn:

	def __init__(self):
		self.db_conn = cx_Oracle.connect(oracle_connect_uri)
		logger.info("Connect to Oracle success")
		
	def sync_exec(self,mysqlConn):
		cursor = self.db_conn.cursor()
		
		list = {'VIEW_FM_CODE_PUB':'FM_CODE_PUB','VIEW_T_ORG_DEPT':'t_Org_Dept','VIEW_T_STF_BASE':'t_stf_base','VIEW_T_STF_POSITION':'t_stf_position'}
		flag = True
		for OracleTable,MySQLTable in list.items():
			sql_statement = '''select * from %s''' % OracleTable
			cursor.execute(sql_statement)

			insert_data = ""
			while True:
				row = cursor.fetchone()
				if row is None:
					break
				data = "("
				for col in row:
					if col is None:
						data += 'NULL'
					else:
						data += """'%s'""" % str(col)
					data += ','
				data = data[:-1]
				data += ')'
				insert_data += data
				insert_data += ','
			insert_data = insert_data[:-1]
			
			ret = mysqlConn.sysn_insert(MySQLTable,insert_data)
			if ret is True:
				logger.info("INSERT Table: %s" % MySQLTable)
			else:
				flag = False
				logger.error("INSERT FAILED")
		if flag is True:
			os.system("/usr/local/whistle/Orable2Mysql/dataSyncClient/bin/restart.sh")

		cursor.close()	
	
	def finish(self):
		self.db_conn.close()
		logger.info("Disconnect to Oracle success")

class MySQLConn:
	
	def __init__(self):
		self.db_conn = MySQLdb.connect("172.16.10.135", "whistle", "MzAyMzAwMTJ", "whistle_cas", charset='utf8')
		logger.info("Connect to MySQL success")
		
	def sysn_create_table(self):
		cursor = self.db_conn.cursor()
		
		try:
			cursor.execute("drop table if exists FM_CODE_PUB")
			sql = """
			CREATE TABLE FM_CODE_PUB(
				`ID` int(22) NOT NULL AUTO_INCREMENT,
				`CODE` VARCHAR(255),
				`NAME` VARCHAR(255),
				`CODE_DEFINE` VARCHAR(255),
				`UP_` VARCHAR(255),
				`ORDER_NUM` double,
				PRIMARY KEY(`ID`)
				) ENGINE=InnoDB DEFAULT CHARSET=utf8
			"""
			cursor.execute(sql)
			
			cursor.execute("drop table if exists t_Org_Dept")
			sql = """
			CREATE TABLE t_Org_Dept(
				`DEPT_ID` int(22) NOT NULL AUTO_INCREMENT,
				`DEPT_CODE` VARCHAR(255),
				`DEPT_NAME` VARCHAR(255),
				`DEPT_SHORTNAME` VARCHAR(255),
				`DEPT_ENAME` VARCHAR(255),
				`PARENT_ID` VARCHAR(255),
				`DEPT_LEVEL` VARCHAR(255),
				`DEPT_SORT` VARCHAR(255),
				`DEPT_PROP` VARCHAR(255),
				`DEPT_ADDRESS` VARCHAR(255),
				`DEPT_DESCRIBE` VARCHAR(255),
				`DEPT_PHONE` VARCHAR(255),
				`DEPT_FAX` VARCHAR(255),
				`REMARK` VARCHAR(255),
				`ESTABLISH_FILECODE` VARCHAR(255),
				`ESTABLISH_DATE` VARCHAR(255),
				`REMOVE_FLAG` VARCHAR(255),
				`IS_DIR` VARCHAR(255),
				`IS_REAL_` VARCHAR(255),
				`ORDER_NUM_` double,
				`Z_CHU_` int(22),
				`F_CHU_` int(22),
				`Z_KE_` int(22),
				`F_KE_` int(22),
				`Z_GAO_` int(22),
				`F_GAO_` int(22),
				`Z_J_` int(22),
				`C_J_` int(22),
				`Y_J_` int(22),
				PRIMARY KEY(`DEPT_ID`)
				) ENGINE=InnoDB DEFAULT CHARSET=utf8
			"""
			cursor.execute(sql)
			
			cursor.execute("drop table if exists t_stf_base")
			sql = """
			CREATE TABLE t_stf_base(
				`NAME` VARCHAR(255),
				`NAME_SPELL` VARCHAR(255),
				`SEX` VARCHAR(255),
				`BIRTHDAY` timestamp,
				`NATION` VARCHAR(255),
				`NATIVE_PLACE` VARCHAR(255),
				`ID_CODE` VARCHAR(255),
				`POLITY` VARCHAR(255),
				`POLITY_DATE` timestamp,
				`WORK_DATE` timestamp,
				`JOIN_U_DATE` timestamp,
				`SPECIALITY` VARCHAR(255),
				`STATUS` VARCHAR(255),
				`POSTAL_ADDRESS` VARCHAR(255),
				`POSTCODE` VARCHAR(255),
				`EMAIL` VARCHAR(255),
				`PHONE` VARCHAR(255),
				`FAMILY_ADDRESS` VARCHAR(255),
				`MOBILE_PHONE` VARCHAR(255),
				`HOME_PHONE` VARCHAR(255),
				`REMARK` VARCHAR(255),
				`EMPLOYEE_CODE` VARCHAR(255),
				`DEPT_ID` VARCHAR(255),
				`THIRD_UNIT` VARCHAR(255),
				`STAFF_TYPE_` VARCHAR(255),
				`MARRIAGE_` VARCHAR(255),
				`PHOTO_` VARCHAR(255),
				`EMPLOYEE_ID` int(22) NOT NULL AUTO_INCREMENT,
				`RESIDENCE_` VARCHAR(255),
				`SUBJECT_` VARCHAR(255),
				`OLD_NAME_` VARCHAR(255),
				`FROM_NATION_` VARCHAR(255),
				`SUBJECT_SECOND_` VARCHAR(255),
				`IDCARD_TYPE_` VARCHAR(255),
				`HOME_POSTCODE_` VARCHAR(255),
				`POST_STATUS_` VARCHAR(255),
				`LAST_UNIT_` VARCHAR(255),
				`STAFF_SORT_` VARCHAR(255),
				`BIRTHAREA_` VARCHAR(255),
				`RETIRE_DATE2_` timestamp,
				`AGE_` int(22),
				`WORK_AGE_` int(22),
				`ABSENCE_AGE_` int(22),
				`STAFF_FROM_` VARCHAR(255),
				`XYJG_` VARCHAR(255),
				`DEPT_TYPE_` VARCHAR(255),
				`NAME_T_` VARCHAR(255),
				`JOIN_U_WORK_` VARCHAR(255),
				`SECOND_LANGUAGE_` VARCHAR(255),
				`HJSZD_` VARCHAR(255),
				`JJLXR_` VARCHAR(255),
				`EMAIL_T_` VARCHAR(255),
				`SYQZQKH_` VARCHAR(255),
				`SYQMQKH_` VARCHAR(255),
				`SYQZZSJ_` timestamp,
				`NJSJ_` VARCHAR(255),
				`DASZD_` VARCHAR(255),
				`LABORATORY_` VARCHAR(255),
				`IS_GY_` VARCHAR(255),
				`HT_TYPE_` VARCHAR(255),
				`FDQK_` VARCHAR(255),
				`BANK_NAME_` VARCHAR(255),
				`BANK_` VARCHAR(255),
				`BANK_CARD_` VARCHAR(255),
				PRIMARY KEY(`EMPLOYEE_ID`)
				) ENGINE=InnoDB DEFAULT CHARSET=utf8
			"""
			cursor.execute(sql)
			
			cursor.execute("drop table if exists t_stf_position")
			sql = """
			CREATE TABLE t_stf_position(
				`EMPLOYEE_ID` VARCHAR(255),
				`POST_SORT` VARCHAR(255),
				`POST_LEVEL` VARCHAR(255),
				`DEPT_ID` VARCHAR(255),
				`START_DATE` timestamp,
				`END_DATE` timestamp,
				`REMARK` VARCHAR(255),
				`IS_LAST_` VARCHAR(255),
				`POST_NUN_` VARCHAR(255),
				`ID_` int(22) NOT NULL AUTO_INCREMENT,
				`SCHOOL_POST_TYPE_` VARCHAR(255),
				`IS_MAIN_` VARCHAR(255),
				PRIMARY KEY(`ID_`)
				) ENGINE=InnoDB DEFAULT CHARSET=utf8
			"""
			cursor.execute(sql)
			cursor.close()
		except Exception,e:
			logger.error("create tables Failed <%s>" % e)
		

	def sysn_insert(self,table,data):
		cursor = self.db_conn.cursor()
		try:
			sql_statement = """insert into %s values %s""" % (table,data)
			cursor.execute(sql_statement)
			self.db_conn.commit()
			cursor.close()
			return True
		except Exception,e:
			logger.error("insert into MySQL Failed <%s>" % e)
			logger.error("FailedTable:::%s" % table)
			return False
		
	
	def finish(self):
		self.db_conn.close()
		logger.info("Disconnect to MySQL success")


if __name__ == '__main__':
	try:
		reload(sys)
		sys.setdefaultencoding('utf-8')
		os.environ['NLS_LANG'] = 'SIMPLIFIED CHINESE_CHINA.UTF8'
		logger.info("Start syncserver ...")
		oracleConn = OracleConn()
		mysqlConn = MySQLConn()
		mysqlConn.sysn_create_table()
		oracleConn.sync_exec(mysqlConn)
		oracleConn.finish()
		mysqlConn.finish()
		logger.info("Syncserver stop ...")
	except Exception,e:
		logger.error("Oracle2MySQL Failed!")
