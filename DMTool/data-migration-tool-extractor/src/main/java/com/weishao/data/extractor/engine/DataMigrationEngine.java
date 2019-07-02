package com.weishao.data.extractor.engine;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;
import org.apache.commons.dbcp2.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowCallbackHandler;
import com.weishao.dmtool.DMTool;
import com.weishao.dmtool.core.JdbcConnector;
import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.dmtool.core.factory.DataSourceFactory;

public class DataMigrationEngine {

	protected static Log logger = LogFactory.getLog(DataMigrationEngine.class);

	private DMTool dmtool;
	private JdbcConnector jdbcSourceConnector;
	private JdbcConnector jdbcTargetConnector;

	public DataMigrationEngine(DMTool dm, JdbcConnector conn) {
		this.dmtool = dm;

		this.jdbcSourceConnector = dm.getSourceJdbcConnector();
		this.jdbcTargetConnector = conn;
	}

	public void doMigration(Map<String, String> tableMappers, boolean createIfNotExist, boolean truncateTable) throws Exception {
		BasicDataSource sourceDataSouce = DataSourceFactory.getDataSource(this.jdbcSourceConnector);
		BasicDataSource targetDataSouce = DataSourceFactory.getDataSource(this.jdbcTargetConnector);

		for (Map.Entry<String, String> entry : tableMappers.entrySet()) {
			String sourceTableName = entry.getKey();
			String targetTableName = entry.getValue();
			if (null == targetTableName || "".equals(targetTableName) || null == sourceTableName
					|| "".equals(sourceTableName)) {
				throw new RuntimeException("Invalid parameters [tableMappers],target table name can not be empty!");
			}

			// 表结构抽取与映射
			List<TableColumnDesc> fieldNames = this.dmtool.querySourceTableColumnDesc(sourceTableName);
			List<String> primaryKeys = this.dmtool.querySourceTablePrimaryKeys(sourceTableName);
			String createTableSql = this.dmtool.getMysqlCreateSentence(fieldNames, primaryKeys, targetTableName,
					createIfNotExist);
			JdbcTemplate sourceJdbcTemplate = new JdbcTemplate(sourceDataSouce);
			JdbcTemplate targetJdbcTemplate = new JdbcTemplate(targetDataSouce);
			targetJdbcTemplate.execute(createTableSql);
			logger.info("create table in target database:" + createTableSql);
			if (truncateTable) {
				targetJdbcTemplate.execute(String.format("TRUNCATE TABLE `%s`", targetTableName));
			}

			// 数据传送
			String quotation=this.dmtool.getQuotationChar();
			dataTransfer(sourceJdbcTemplate, targetJdbcTemplate, sourceTableName, targetTableName, fieldNames,quotation);
		}
	}

	private void dataTransfer(JdbcTemplate sourceJdbcTemplate,JdbcTemplate targetJdbcTemplate,String sourceTableName,String targetTableName,List<TableColumnDesc> fieldNames,String quotation) {
		StringBuilder keysSource=new StringBuilder();
		StringBuilder keysTarget=new StringBuilder();
		StringBuilder vals=new StringBuilder();
		for(int i=0;i<fieldNames.size();++i) {
			TableColumnDesc column=fieldNames.get(i);
			keysSource.append(quotation);
			keysSource.append(column.getFieldName());
			keysSource.append(quotation);
			
			keysTarget.append("`");
			keysTarget.append(column.getFieldName());
			keysTarget.append("`");
			
			vals.append("?");
			if(i!=fieldNames.size()-1) {
				keysSource.append(",");
				keysTarget.append(",");
				vals.append(",");
			}
		}
		
		String sqlQuery=String.format("select %s from %s",keysSource.toString(),sourceTableName);
		logger.info("query sql from source database:" + sqlQuery);
		String sqlInsert=String.format("replace into `%s`(%s) values(%s)",targetTableName,keysTarget.toString(),vals.toString());
		logger.info("insert sql to target database:" + sqlQuery);
		long startTime = System.currentTimeMillis();
		sourceJdbcTemplate.query(sqlQuery, new RowCallbackHandler() {
			int totalCount=0;
			public void processRow(ResultSet rs) throws SQLException {
				Object values[]=new Object[fieldNames.size()];
				int k=0;
				ResultSetMetaData metaData = rs.getMetaData();
				for (int j = 1; j <= metaData.getColumnCount(); ++j) {
					values[k++]=rs.getObject(j);
				}
				
				targetJdbcTemplate.update(sqlInsert,values);
				totalCount++;
				
				if(0==totalCount%10000) {
					logger.info("handle data total count:" + totalCount);
				}
			}
		});
		long endTime = System.currentTimeMillis();
		float seconds = (endTime - startTime) / 1000F;
		logger.info("data transfer ok,elipse seconds:"+Float.toString(seconds));
	}
}
