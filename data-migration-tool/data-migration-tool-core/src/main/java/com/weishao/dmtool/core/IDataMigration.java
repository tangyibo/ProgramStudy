package com.weishao.dmtool.core;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import javax.sql.DataSource;

import com.weishao.dmtool.core.except.UnsupportDatabaseTypeException;
import com.weishao.dmtool.core.except.UnsupportJdbcTypeMapper;

public interface IDataMigration {
	public void setSourceDatabase(JdbcConnector connector) throws UnsupportDatabaseTypeException;

	public JdbcConnector getSourceJdbcConnector();

	public DataSource getSourceDataSource();

	public Map<String, String> querySourceDatabaseTables() throws SQLException, UnsupportDatabaseTypeException;

	public List<TableColumnDesc> querySourceTableColumnDesc(String sourceTableName)
			throws UnsupportDatabaseTypeException;

	public List<String> querySourceTablePrimaryKeys(String sourceTableName) throws SQLException;

	public String getMysqlCreateTableSql(String sourceTableName, String targetTableName, boolean createIfNotExist)
			throws UnsupportDatabaseTypeException, SQLException, UnsupportJdbcTypeMapper;

	public String getMysqlCreateSentence(List<TableColumnDesc> fieldNames, List<String> primaryKeys, String tableName,
			boolean createIfNotExist) throws UnsupportJdbcTypeMapper;

	public void close();
}
