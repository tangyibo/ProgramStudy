package com.weishao.migration.tool.server.entity;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;

@ApiModel(value = "数据库连接与表", description = "数据库连接与表参数")
public class QueryTableParams extends ConnectionParams {

	@ApiModelProperty(value = "源表名", required = true, example = "ofuser_cas")
	private String src_table;
	
	@ApiModelProperty(value = "新表名", required = true, example = "new_ofuser_cas")
	private String dest_table;

	public String getSrc_table() {
		return src_table;
	}

	public void setSrc_table(String src_table) {
		this.src_table = src_table;
	}

	public String getDest_table() {
		return dest_table;
	}

	public void setDest_table(String dest_table) {
		this.dest_table = dest_table;
	}

	@Override
	public String toString() {
		return "QueryTableParams [src_table=" + src_table + ", dest_table=" + dest_table + "] "+ super.toString();
	}
	
}
