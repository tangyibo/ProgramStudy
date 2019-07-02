package com.weishao.dmtool;

import java.sql.SQLException;

import com.weishao.dmtool.core.DatabaseType;
import com.weishao.dmtool.core.JdbcConnector;
import com.weishao.dmtool.core.except.UnsupportDatabaseTypeException;
import com.weishao.dmtool.core.except.UnsupportJdbcTypeMapper;

import junit.framework.TestCase;

/**
 * Unit test for simple App.
 */
public class AppTest extends TestCase {
	
	private DMTool dm=null;
	
	@Override
	public void setUp() throws Exception{
		this.dm = new DMTool();
	}

	@Override
    public void tearDown() throws Exception
    {
        this.dm.close();
    }
	
	/**
	 * MySQL数据库测试用例
	 */
	public void testFromMySQL() {
		try {
			JdbcConnector connector1=new JdbcConnector(DatabaseType.MYSQL);
			connector1.setHostAddress("172.16.90.210");
			connector1.setServerPort(3306);
			connector1.setDbName("tangyibo");
			connector1.setUserName("tangyibo");
			connector1.setPassword("tangyibo");
			dm.setSourceDatabase(connector1);
			System.out.println(dm.getMysqlCreateTableSql("ofuser_cas", "new_ofuser_cas", true));
		} catch (UnsupportDatabaseTypeException | SQLException | UnsupportJdbcTypeMapper e) {
			e.printStackTrace();
			assertTrue(false);
		}
		

		assertTrue(true);
	}

	/**
	 * Oracle数据库测试用例
	 */
	public void testFromOracle() {
		try {
			JdbcConnector connector1 = new JdbcConnector(DatabaseType.ORACLE);
			connector1.setHostAddress("172.16.90.252");
			connector1.setServerPort(1521);
			connector1.setDbName("orcl");
			connector1.setUserName("yi_bo");
			connector1.setPassword("yi_bo");
			dm.setSourceDatabase(connector1);
			System.out.println(dm.getMysqlCreateTableSql("OFUSER_CAS", "new_OFUSER_CAS", true));
		} catch (UnsupportDatabaseTypeException | SQLException | UnsupportJdbcTypeMapper e) {
			e.printStackTrace();
			assertTrue(false);
		}

		assertTrue(true);
	}
	
	/**
	 * SQLServer数据库测试用例
	 */
	public void testFromSqlServer() {
		try {
			JdbcConnector connector1=new JdbcConnector(DatabaseType.SQLSERVER);
			connector1.setHostAddress("172.16.90.166");
			connector1.setServerPort(1433);
			connector1.setDbName("smartbi");
			connector1.setUserName("smartbi");
			connector1.setPassword("smartbi");
			dm.setSourceDatabase(connector1);
			System.out.println(dm.getMysqlCreateTableSql("all_field", "new_all_field", true));
		} catch (UnsupportDatabaseTypeException | SQLException | UnsupportJdbcTypeMapper e) {
			e.printStackTrace();
			assertTrue(false);
		}
		
		assertTrue(true);
	}
	
}
