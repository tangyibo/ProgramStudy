package com.weishao.migration.tool.server.controller;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import com.weishao.dmtool.DMTool;
import com.weishao.dmtool.core.DatabaseType;
import com.weishao.dmtool.core.JdbcConnector;
import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.migration.tool.server.entity.ConnectionParams;
import com.weishao.migration.tool.server.entity.QueryTableParams;

import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiParam;

@Api(value = "查询接口相关操作", tags = { "restful" })
@RestController
@RequestMapping(value = "")
public class RestfulController {

	protected static Log logger = LogFactory.getLog(RestfulController.class);

	protected static Map<String, DatabaseType> dbTypeMapper;

	static {
		dbTypeMapper = new HashMap<String, DatabaseType>();
		dbTypeMapper.put("mysql", DatabaseType.MYSQL);
		dbTypeMapper.put("oracle", DatabaseType.ORACLE);
		dbTypeMapper.put("mssql", DatabaseType.SQLSERVER);
	}

	@RequestMapping(value = "/query_table_list", method = RequestMethod.POST, produces = MediaType.APPLICATION_JSON_VALUE)
	@ResponseBody
	@ApiOperation(value = "获取库中的表", notes = "根据指定的数据库连接信息获取库中的表")
	public Map<String, Object> queryTableLists(
			@RequestBody @ApiParam(name = "数据库连接对象", value = "传入json格式", required = true) ConnectionParams param) {
		logger.info("query_table_list() called and param : " + param.toString());
		try {
			if (dbTypeMapper.containsKey(param.getType())) {
				DMTool dmt = new DMTool();
				JdbcConnector conn = new JdbcConnector(dbTypeMapper.get(param.getType()));
				conn.setHostAddress(param.getHost());
				conn.setServerPort(param.getPort());
				conn.setDbName(param.getDbname());
				conn.setUserName(param.getUser());
				conn.setPassword(param.getPasswd());
				dmt.setSourceDatabase(conn);

				List<Map<String, String>> data = new ArrayList<Map<String, String>>();
				Map<String, String> ret = dmt.querySourceDatabaseTables();
				for (Map.Entry<String, String> entry : ret.entrySet()) {
					Map<String, String> item = new HashMap<String, String>();
					item.put("table_name", entry.getKey());
					item.put("table_type", entry.getValue());
					data.add(item);
				}
				return success(data);
			} else {
				throw new RuntimeException("Unsuppot database type string");
			}
		} catch (Exception e) {
			return failed(-1, e.getMessage());
		}
	}

	@RequestMapping(value = "/query_table_info", method = RequestMethod.POST, produces = MediaType.APPLICATION_JSON_VALUE)
	@ResponseBody
	@ApiOperation(value = "获取表的信息", notes = "根据指定的数据库连接信息获取表的信息")
	public Map<String, Object> queryTableInfo(
			@RequestBody @ApiParam(name = "数据库连接对象与表名称", value = "传入json格式", required = true) QueryTableParams param) {
		logger.info("query_table_info() called and param : " + param.toString());

		try {
			if (dbTypeMapper.containsKey(param.getType())) {
				DMTool dmt = new DMTool();
				JdbcConnector conn = new JdbcConnector(dbTypeMapper.get(param.getType()));
				conn.setHostAddress(param.getHost());
				conn.setServerPort(param.getPort());
				conn.setDbName(param.getDbname());
				conn.setUserName(param.getUser());
				conn.setPassword(param.getPasswd());
				dmt.setSourceDatabase(conn);

				String sourceTableName = param.getSrc_table();
				String targetTableName = param.getDest_table();
				List<TableColumnDesc> filedsList = dmt.querySourceTableColumnDesc(sourceTableName);
				List<String> columns = new ArrayList<String>();
				for (TableColumnDesc one : filedsList) {
					columns.add(one.getFieldName());
				}

				Map<String, Object> ret = new HashMap<String, Object>();
				ret.put("primary_key", dmt.querySourceTablePrimaryKeys(sourceTableName));
				ret.put("columns", columns);
				ret.put("create_sql", dmt.getMysqlCreateTableSql(sourceTableName, targetTableName, true));
				return success(ret);
			} else {
				throw new RuntimeException("Unsuppot database type string");
			}
		} catch (Exception e) {
			return failed(-1, e.getMessage());
		}

	}

	/**
	 * 成功响应
	 * 
	 * @param data
	 * @return
	 */
	protected Map<String, Object> success(Object data) {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("errcode", 0);
		map.put("errmsg", "success");
		map.put("data", data);
		logger.info("[Response] " + map.toString());
		return map;
	}

	/**
	 * 失败响应
	 * 
	 * @param errno  失败的错误码
	 * @param reason 失败的原因
	 * @return
	 */
	protected Map<String, Object> failed(long errno, String reason) {
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("errcode", errno);
		map.put("errmsg", reason);
		logger.info("[Response] " + map.toString());
		return map;
	}
}
