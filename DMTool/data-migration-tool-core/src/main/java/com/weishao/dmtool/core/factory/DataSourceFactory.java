package com.weishao.dmtool.core.factory;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.dbcp2.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.weishao.dmtool.core.DatabaseType;
import com.weishao.dmtool.core.JdbcConnector;
import com.weishao.dmtool.core.except.UnsupportDatabaseTypeException;

public class DataSourceFactory {
	
	protected static Log logger = LogFactory.getLog(DataSourceFactory.class);

	public static Map<String, String> databaseTypes = new HashMap<String, String>();

	static {
		databaseTypes.put(DatabaseType.ORACLE.name().toUpperCase(), "oracle.jdbc.driver.OracleDriver");
		databaseTypes.put(DatabaseType.MYSQL.name().toUpperCase(), "com.mysql.jdbc.Driver");
		databaseTypes.put(DatabaseType.SQLSERVER.name().toUpperCase(), "com.microsoft.sqlserver.jdbc.SQLServerDriver");
	}

	public static BasicDataSource getDataSource(JdbcConnector connector) throws UnsupportDatabaseTypeException {
		if (DataSourceFactory.databaseTypes.containsKey(connector.getDbType().name().toUpperCase())) {
			String driverName = DataSourceFactory.databaseTypes.get(connector.getDbType().name().toUpperCase());
			BasicDataSource ds = new BasicDataSource();
			ds.setDriverClassName(driverName);
			ds.setUrl(connector.toJdbcUrlString());
			ds.setUsername(connector.getUserName());
			ds.setPassword(connector.getPassword());
			ds.setInitialSize(5);
			return ds;
		}

		logger.debug(String.format("Unsupport database type:[%s]", connector.getDbType().name()));
		throw new UnsupportDatabaseTypeException(String.format("Unsupport database type:[%s]", connector.getDbType().name()));
	}

	public static void closeDataSource(BasicDataSource dataSource) {
		try {
			dataSource.close();
		} catch (SQLException e) {
			logger.debug("Error where close datasource' - " + e.getMessage());
		}
	}

}
