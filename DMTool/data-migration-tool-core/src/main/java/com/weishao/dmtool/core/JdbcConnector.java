package com.weishao.dmtool.core;

/**
 * JDBC连接器
 * @author tang
 *
 */
public class JdbcConnector {

	private DatabaseType dbType;
	private String hostAddress;
	private int serverPort;
	private String dbName;
	private String userName;
	private String password;

	public JdbcConnector(DatabaseType dbType) {
		this.dbType = dbType;
		if (dbType == DatabaseType.MYSQL) {
			this.serverPort = 3306;
		} else if (dbType == DatabaseType.ORACLE) {
			this.serverPort = 1521;
		} else if (dbType == DatabaseType.SQLSERVER) {
			this.serverPort = 1433;
		} else {
			this.serverPort = 0;
		}

		this.hostAddress = "localhost";
	}

	public DatabaseType getDbType() {
		return dbType;
	}

	public String getHostAddress() {
		return hostAddress;
	}

	public void setHostAddress(String hostAddress) {
		this.hostAddress = hostAddress;
	}

	public int getServerPort() {
		return serverPort;
	}

	public void setServerPort(int serverPort) {
		this.serverPort = serverPort;
	}

	public String getDbName() {
		return dbName;
	}

	public void setDbName(String dbName) {
		this.dbName = dbName;
	}
	
	public void setUserName(String userName) {
		this.userName=userName;
	}
	
	public String getUserName() {
		return this.userName;
	}
	
	public void setPassword(String password) {
		this.password=password;
	}

	public String getPassword() {
		return this.password;
	}
	
	public String toJdbcUrlString() {
		if (dbType == DatabaseType.MYSQL) {
			return String.format(
					"jdbc:mysql://%s:%d/%s?characterEncoding=utf8&useSSL=true&nullCatalogMeansCurrent=true",
					hostAddress, serverPort, dbName);
		} else if (dbType == DatabaseType.ORACLE) {
			return String.format("jdbc:oracle:thin:@%s:%d:%s", hostAddress, serverPort, dbName);
		} else if (dbType == DatabaseType.SQLSERVER) {
			return String.format("jdbc:sqlserver://%s:%d;DatabaseName=%s", hostAddress, serverPort, dbName);
		} else {
			return null;
		}
	}
}
