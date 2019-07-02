package com.weishao.dmtool.core.factory;

import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import org.apache.commons.dbcp2.BasicDataSource;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import org.springframework.jdbc.support.rowset.SqlRowSet;
import org.springframework.jdbc.support.rowset.SqlRowSetMetaData;

import com.weishao.dmtool.core.DatabaseType;
import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.dmtool.core.except.UnsupportDatabaseTypeException;

public class SqlTemplateFactory {

	protected static Log logger = LogFactory.getLog(SqlTemplateFactory.class);

	private static Map<String, String> sqlColumnFieldsTemplates = new HashMap<String, String>();

	static {
		// 获取列字段描述相关SQL模版
		sqlColumnFieldsTemplates.put(DatabaseType.MYSQL.name().toLowerCase(), "select * from  `%s` limit 1 ");
		sqlColumnFieldsTemplates.put(DatabaseType.ORACLE.name().toLowerCase(), "SELECT * FROM \"%s\" where rownum<1 ");
		sqlColumnFieldsTemplates.put(DatabaseType.SQLSERVER.name().toLowerCase(), "select top 1 * from [%s] ");
	}

	private static String getColumnFieldsTemplateSql(DatabaseType sourceDbType, String tableName)
			throws UnsupportDatabaseTypeException {
		if (sqlColumnFieldsTemplates.containsKey(sourceDbType.name().toLowerCase())) {
			return String.format(sqlColumnFieldsTemplates.get(sourceDbType.name().toLowerCase()), tableName);
		}

		logger.debug(String.format("Unsupport database type:[%s]", sourceDbType.name()));
		throw new UnsupportDatabaseTypeException(String.format("Unsupport database type:[%s]", sourceDbType.name()));
	}

	public static Map<String, String> getDatabaseTablesList(DatabaseType sourceDbType, BasicDataSource dataSource)
			throws SQLException, UnsupportDatabaseTypeException {
		if (sqlColumnFieldsTemplates.containsKey(sourceDbType.name().toLowerCase())) {
			Connection connection = dataSource.getConnection();
			DatabaseMetaData dbMetaData = connection.getMetaData();

			String[] types = { "table", "view" };
			ResultSet rs = dbMetaData.getTables(null, null, null, types);
			Map<String, String> results = new HashMap<String, String>();
			while (rs.next()) {
				String tableName = rs.getString("TABLE_NAME"); // 表名
				String tableType = rs.getString("TABLE_TYPE").toLowerCase(); // 表类型
				results.put(tableName, tableType);
			}

			return results;
		}

		logger.debug(String.format("Unsupport database type:[%s]", sourceDbType.name()));
		throw new UnsupportDatabaseTypeException(String.format("Unsupport database type:[%s]", sourceDbType.name()));
	}

	public static List<TableColumnDesc> querySourceTableColumnDesc(DatabaseType sourceDbType,
			BasicDataSource dataSource, String tableName) throws UnsupportDatabaseTypeException {
		String sql = SqlTemplateFactory.getColumnFieldsTemplateSql(sourceDbType, tableName);

		List<TableColumnDesc> results = new ArrayList<TableColumnDesc>();
		JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);

		// 当表或视图中有数据时
		jdbcTemplate.query(sql, new RowCallbackHandler() {

			public void processRow(ResultSet rs) throws SQLException {
				ResultSetMetaData m = rs.getMetaData();
				for (int i = 1; i <= m.getColumnCount(); ++i) {
					TableColumnDesc columnDesc = new TableColumnDesc();
					columnDesc.setFieldName(m.getColumnName(i));
					columnDesc.setLabalName(m.getColumnName(i));
					columnDesc.setFieldType(m.getColumnType(i));
					columnDesc.setFieldTypeName(m.getColumnTypeName(i));
					columnDesc.setFiledTypeClassName(m.getColumnClassName(i));
					columnDesc.setDisplaySize(m.getColumnDisplaySize(i));
					columnDesc.setPrecisionSize(m.getPrecision(i));
					columnDesc.setScaleSize(m.getScale(i));
					columnDesc.setAutoIncrement(m.isAutoIncrement(i));
					columnDesc.setNullable(m.isNullable(i) == 1 ? true : false);
					results.add(columnDesc);
				}
			}

		});

		if (results.isEmpty()) {
			SqlRowSet rowSet = jdbcTemplate.queryForRowSet(sql);
			SqlRowSetMetaData m = rowSet.getMetaData();
			int columnCount = m.getColumnCount();
			for (int i = 1; i <= columnCount; i++) {
				TableColumnDesc columnDesc = new TableColumnDesc();
				columnDesc.setFieldName(m.getColumnName(i));
				columnDesc.setLabalName(m.getColumnName(i));
				columnDesc.setFieldType(m.getColumnType(i));
				columnDesc.setFieldTypeName(m.getColumnTypeName(i));
				columnDesc.setFiledTypeClassName(m.getColumnClassName(i));
				columnDesc.setDisplaySize(m.getColumnDisplaySize(i));
				columnDesc.setPrecisionSize(m.getPrecision(i));
				columnDesc.setScaleSize(m.getScale(i));
				columnDesc.setAutoIncrement(false);
				columnDesc.setNullable(true);
				results.add(columnDesc);
			}
		}

		return results;
	}

	public static List<String> querySourceTablePrimaryKeys(BasicDataSource dataSource, String tableName)
			throws SQLException {
		Connection connection = dataSource.getConnection();
		DatabaseMetaData dbMetaData = connection.getMetaData();
		ResultSet rs = dbMetaData.getPrimaryKeys(null, null, tableName);
		List<String> results = new ArrayList<String>();

		while (rs.next()) {
			results.add(rs.getString("COLUMN_NAME"));
		}

		return results;
	}
}
