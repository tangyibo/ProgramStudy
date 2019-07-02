package com.weishao.migration.tool.server.entity;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;

@ApiModel(value = "数据库连接参数", description = "数据库连接参数")
public class ConnectionParams {
	@ApiModelProperty(value = "数据库类型", required = true, example = "mysql")
	private String type;

	@ApiModelProperty(value = "主机IP地址", required = true, example = "172.16.90.210")
	private String host;

	@ApiModelProperty(value = "端口号", required = true, example = "3306")
	private int port;

	@ApiModelProperty(value = "用户名", required = true, example = "tangyibo")
	private String user;

	@ApiModelProperty(value = "密码", required = true, example = "tangyibo")
	private String passwd;

	@ApiModelProperty(value = "库名称", required = true, example = "tangyb")
	private String dbname;

	@ApiModelProperty(value = "字符集", required = true, example = "utf-8")
	private String charset;

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public String getHost() {
		return host;
	}

	public void setHost(String host) {
		this.host = host;
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public String getUser() {
		return user;
	}

	public void setUser(String user) {
		this.user = user;
	}

	public String getPasswd() {
		return passwd;
	}

	public void setPasswd(String passwd) {
		this.passwd = passwd;
	}

	public String getDbname() {
		return dbname;
	}

	public void setDbname(String dbname) {
		this.dbname = dbname;
	}

	public String getCharset() {
		return charset;
	}

	public void setCharset(String charset) {
		this.charset = charset;
	}

	@Override
	public String toString() {
		return "ConnectionParams [type=" + type + ", host=" + host + ", port=" + port + ", user=" + user + ", passwd="
				+ passwd + ", dbname=" + dbname + ", charset=" + charset + "]";
	}

	
}
