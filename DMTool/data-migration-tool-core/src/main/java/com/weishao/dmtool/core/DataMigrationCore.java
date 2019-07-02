package com.weishao.dmtool.core;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.sql.DataSource;

import org.apache.commons.dbcp2.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.dmtool.core.IDataMigration;
import com.weishao.dmtool.core.except.UnsupportDatabaseTypeException;
import com.weishao.dmtool.core.except.UnsupportJdbcTypeMapper;
import com.weishao.dmtool.core.factory.DataSourceFactory;
import com.weishao.dmtool.core.factory.JdbcTypeMapFactory;
import com.weishao.dmtool.core.factory.SqlTemplateFactory;

/**
 * 数据迁移核心服务
 * @author tang
 *
 */
public class DataMigrationCore implements IDataMigration{
	
	protected static Log logger = LogFactory.getLog(DataMigrationCore.class);
	
	private JdbcConnector jdbcSourceConnector;
	
	private BasicDataSource dataSourceofSource;
	
	public DataMigrationCore() {
		this.jdbcSourceConnector=null;
		
		this.dataSourceofSource=null;
	}
	
	@Override
	public void setSourceDatabase(JdbcConnector connector) throws UnsupportDatabaseTypeException {
		if(null!=this.dataSourceofSource) {
			try {
				this.dataSourceofSource.close();
			} catch (SQLException e) {
			}
		}
		
		this.jdbcSourceConnector=connector;
		this.dataSourceofSource = DataSourceFactory.getDataSource(this.jdbcSourceConnector);
	}
	
	@Override
	public JdbcConnector getSourceJdbcConnector() {
		return this.jdbcSourceConnector;
	}
	
	@Override
	public DataSource getSourceDataSource() {
		return this.dataSourceofSource;
	}
	
	@Override
	public Map<String,String> querySourceDatabaseTables() throws SQLException, UnsupportDatabaseTypeException{
		return SqlTemplateFactory.getDatabaseTablesList(this.jdbcSourceConnector.getDbType(), this.dataSourceofSource);
	}
	
	@Override
	public List<TableColumnDesc> querySourceTableColumnDesc(String sourceTableName) throws UnsupportDatabaseTypeException{
		return SqlTemplateFactory.querySourceTableColumnDesc(this.jdbcSourceConnector.getDbType(),this.dataSourceofSource,sourceTableName);
	}
	
	@Override
	public List<String> querySourceTablePrimaryKeys(String sourceTableName) throws SQLException{
		return SqlTemplateFactory.querySourceTablePrimaryKeys(this.dataSourceofSource,sourceTableName);
	}
	
	@Override
	public String getMysqlCreateTableSql(String sourceTableName,String targetTableName,boolean createIfNotExist) throws UnsupportDatabaseTypeException, SQLException, UnsupportJdbcTypeMapper {
		List<TableColumnDesc> fields = SqlTemplateFactory.querySourceTableColumnDesc(this.jdbcSourceConnector.getDbType(),
				this.dataSourceofSource,
				sourceTableName);
		if(fields.isEmpty()) {
			throw new RuntimeException("Table [%s] column field size is 0");
		}
		
		List<String> primaryKeys = SqlTemplateFactory.querySourceTablePrimaryKeys(this.dataSourceofSource,
				sourceTableName);
		return this.getMysqlCreateSentence(fields, primaryKeys, targetTableName, createIfNotExist);
	}
	
	public String getMysqlCreateSentence(List<TableColumnDesc> fieldNames,List<String> primaryKeys,String tableName,boolean createIfNotExist) throws UnsupportJdbcTypeMapper {
		List<String> cols = new ArrayList<String>();
		
		for (TableColumnDesc column : fieldNames) {
			if (primaryKeys.contains(column.getLabalName())) {
				cols.add(String.format("`%s` %s NOT NULL", column.getLabalName(),
						JdbcTypeMapFactory.getMySQLFieldTypeFromJdbcType(column)));
			} else {
				if(column.getFieldTypeName().toUpperCase().equals("TIMESTAMP") || column.getFieldTypeName().toUpperCase().equals("DATETIME")) {
					cols.add(String.format("`%s` %s DEFAULT '1970-01-01 08:00:01'", column.getLabalName(),
							JdbcTypeMapFactory.getMySQLFieldTypeFromJdbcType(column)));
				}else {
					cols.add(String.format("`%s` %s DEFAULT NULL", column.getLabalName(),
							JdbcTypeMapFactory.getMySQLFieldTypeFromJdbcType(column)));
				}
			}
		}
		
		if(!primaryKeys.isEmpty()) {
			cols.add(String.format("PRIMARY KEY (%s)", String.join(",", primaryKeys)));
		}

		String ifNotExists="";
		if (createIfNotExist) {
			ifNotExists="IF NOT EXISTS";
		}

		String fieldColumns=String.join(",\n", cols);
		return String.format("CREATE TABLE %s `%s` (\n%s\n) ENGINE=InnoDB DEFAULT CHARSET=utf8 ", ifNotExists, tableName,fieldColumns);
	}
	
	@Override
	public String getQuotationChar() throws UnsupportDatabaseTypeException {
		DatabaseType dbtype=this.jdbcSourceConnector.getDbType();
		if(dbtype==DatabaseType.MYSQL) {
			return "`";
		}else if(dbtype==DatabaseType.ORACLE) {
			return "\"";
		}else if(dbtype==DatabaseType.SQLSERVER) {
			return "\"";
		}else {
			throw new UnsupportDatabaseTypeException(String.format("Unsupport database type:[%s]", dbtype.name()));
		}
	}
	
	@Override
	public void close() {
		DataSourceFactory.closeDataSource(this.dataSourceofSource);
	}
	
}
