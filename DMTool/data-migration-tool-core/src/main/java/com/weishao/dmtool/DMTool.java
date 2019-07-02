package com.weishao.dmtool;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import javax.sql.DataSource;

import com.weishao.dmtool.core.DataMigrationCore;
import com.weishao.dmtool.core.IDataMigration;
import com.weishao.dmtool.core.JdbcConnector;
import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.dmtool.core.except.UnsupportDatabaseTypeException;
import com.weishao.dmtool.core.except.UnsupportJdbcTypeMapper;

public class DMTool 
{
	private IDataMigration core;
	
	public DMTool() {
		core=new DataMigrationCore();
	}
	
	public void setSourceDatabase(JdbcConnector connector) throws UnsupportDatabaseTypeException {
		this.core.setSourceDatabase(connector);
	}
	
	public JdbcConnector getSourceJdbcConnector() {
		return this.core.getSourceJdbcConnector();
	}
	
	public DataSource getSourceDataSource() {
		return this.core.getSourceDataSource();
	}
	
	public Map<String,String> querySourceDatabaseTables() throws SQLException, UnsupportDatabaseTypeException{
		return this.core.querySourceDatabaseTables();
	}
	
	public List<TableColumnDesc> querySourceTableColumnDesc(String sourceTableName)
			throws UnsupportDatabaseTypeException{
		return this.core.querySourceTableColumnDesc(sourceTableName);
	}

	public List<String> querySourceTablePrimaryKeys(String sourceTableName) throws SQLException{
		return this.core.querySourceTablePrimaryKeys(sourceTableName);
	}
	
	public String getMysqlCreateSentence(List<TableColumnDesc> fieldNames, List<String> primaryKeys, String tableName,
			boolean createIfNotExist) throws UnsupportJdbcTypeMapper {
		return this.core.getMysqlCreateSentence(fieldNames, primaryKeys, tableName, createIfNotExist);
	}

	public String getMysqlCreateTableSql(String sourceTableName, String targetTableName, boolean createIfNotExist)
			throws UnsupportDatabaseTypeException, SQLException, UnsupportJdbcTypeMapper{
		return this.core.getMysqlCreateTableSql(sourceTableName, targetTableName, createIfNotExist);
	}
	
	public String getQuotationChar() throws UnsupportDatabaseTypeException{
		return this.core.getQuotationChar();
	}
	
	public void close() {
		this.core.close();
	}
	
}
