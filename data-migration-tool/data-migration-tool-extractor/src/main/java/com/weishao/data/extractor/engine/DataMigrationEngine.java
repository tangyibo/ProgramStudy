package com.weishao.data.extractor.engine;

import java.util.List;
import java.util.Map;
import org.apache.commons.dbcp2.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.jdbc.core.JdbcTemplate;
import com.weishao.dmtool.DMTool;
import com.weishao.dmtool.core.JdbcConnector;
import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.dmtool.core.factory.DataSourceFactory;

import ch.qos.logback.core.FileAppender;

import org.pentaho.di.core.KettleEnvironment;
import org.pentaho.di.core.database.DatabaseMeta;
import org.pentaho.di.core.exception.KettleException;
import org.pentaho.di.trans.Trans;
import org.pentaho.di.trans.TransHopMeta;
import org.pentaho.di.trans.TransMeta;
import org.pentaho.di.trans.step.StepMeta;
import org.pentaho.di.trans.steps.insertupdate.InsertUpdateMeta;
import org.pentaho.di.trans.steps.tableinput.TableInputMeta;
import org.pentaho.di.trans.steps.tableoutput.TableOutputMeta;

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

	public void doInsertUpdate(Map<String, String> tableMappers, boolean createIfNotExist) {

		try {
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
				if (primaryKeys.isEmpty()) {
					throw new RuntimeException(String.format("source table [%s] no primary key", sourceTableName));
				}

				String createTableSql = this.dmtool.getMysqlCreateSentence(fieldNames, primaryKeys, targetTableName,
						createIfNotExist);

				JdbcTemplate jdbcTemplate = new JdbcTemplate(targetDataSouce);
				jdbcTemplate.execute(createTableSql);

				// 数据抽取与映射
				boolean ret = insertUpdateData(sourceTableName, fieldNames, primaryKeys, targetTableName);
				if (ret) {
					logger.debug("extract table data over for table:" + sourceTableName);
				} else {
					logger.error("extract table data error for table:" + sourceTableName);
				}
			}
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}
	

	public void doMigration(Map<String, String> tableMappers, boolean createIfNotExist, boolean dropTable) {

		try {
			BasicDataSource targetDataSouce = DataSourceFactory.getDataSource(this.jdbcTargetConnector);

			for (Map.Entry<String, String> entry : tableMappers.entrySet()) {
				String sourceTableName = entry.getKey();
				String targetTableName = entry.getValue();
				if (null == targetTableName || "".equals(targetTableName) || null == sourceTableName
						|| "".equals(sourceTableName)) {
					throw new RuntimeException("Invalid parameters [tableMappers],target table name can not be empty!");
				}

				// 表结构抽取与映射
				String createTableSql = this.dmtool.getMysqlCreateTableSql(sourceTableName, targetTableName,
						createIfNotExist);

				JdbcTemplate jdbcTemplate = new JdbcTemplate(targetDataSouce);

				if (dropTable) {
					jdbcTemplate.execute(String.format("drop table if exists `%s`", targetTableName));
				}

				jdbcTemplate.execute(createTableSql);

				// 数据抽取与映射
				boolean ret = exctractTableData(sourceTableName, targetTableName, true);
				if (ret) {
					logger.debug("extract table data over for table:" + sourceTableName);
				} else {
					logger.error("extract table data error for table:" + sourceTableName);
				}
			}
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
		}
	}

	public boolean insertUpdateData(String sourceTableName, List<TableColumnDesc> fieldNames, List<String> primaryKeys,
			String targetTableName) throws KettleException {
		try {
			KettleEnvironment.init();
			TransMeta transMeta = new TransMeta();
			transMeta.setName("trans_work");

			DatabaseMeta db_src = new DatabaseMeta("sourceDatabase",
					this.jdbcSourceConnector.getDbType().name().toLowerCase(), "Native",
					this.jdbcSourceConnector.getHostAddress(), this.jdbcSourceConnector.getDbName(),
					String.valueOf(this.jdbcSourceConnector.getServerPort()), this.jdbcSourceConnector.getUserName(),
					this.jdbcSourceConnector.getPassword());

			DatabaseMeta db_dst = new DatabaseMeta("targetDatabase",
					this.jdbcTargetConnector.getDbType().name().toLowerCase(), "Native",
					this.jdbcTargetConnector.getHostAddress(), this.jdbcTargetConnector.getDbName(),
					String.valueOf(this.jdbcTargetConnector.getServerPort()), this.jdbcTargetConnector.getUserName(),
					this.jdbcTargetConnector.getPassword());

			db_dst.addExtraOption(db_dst.getPluginId(), "characterEncoding", "utf8");
			db_src.addExtraOption(db_src.getPluginId(), "zeroDateTimeBehavior", "convertToNull");

			transMeta.addDatabase(db_src);
			transMeta.addDatabase(db_dst);

			DatabaseMeta sourceDatabaseMeta = transMeta.findDatabase("sourceDatabase");
			TableInputMeta t_input = new TableInputMeta();
			t_input.setDatabaseMeta(sourceDatabaseMeta);
			t_input.setSQL(String.format("select * from %s", sourceTableName));
			StepMeta input = new StepMeta("source-table-data-inpurt", t_input);
			transMeta.addStep(input);

			InsertUpdateMeta insertUpdateMeta = new InsertUpdateMeta();
			DatabaseMeta targetDatabaseMeta = transMeta.findDatabase("targetDatabase");
			insertUpdateMeta.setDatabaseMeta(targetDatabaseMeta);
			insertUpdateMeta.setTableName(targetTableName);// 设置操作的表
			// 设置用来查询的关键字
			insertUpdateMeta.setKeyLookup(primaryKeys.toArray(new String[primaryKeys.size()]));
			insertUpdateMeta.setKeyStream(primaryKeys.toArray(new String[primaryKeys.size()]));
			insertUpdateMeta.setKeyStream2(new String[] { "" });// 一定要加上
			insertUpdateMeta.setKeyCondition(new String[] { "=" });

			String[] updatelookup = new String[fieldNames.size()];
			String[] updateStream = new String[fieldNames.size()];
			Boolean[] updateOrNot = new Boolean[fieldNames.size()];
			for (int i = 0; i < fieldNames.size(); ++i) {
				TableColumnDesc field = fieldNames.get(i);
				String columnName = field.getFieldName();
				updatelookup[i] = columnName;
				updateStream[i] = columnName;

				if (primaryKeys.contains(columnName)) {
					updateOrNot[i] = false;
				} else {
					updateOrNot[i] = true;
				}
			}

			insertUpdateMeta.setUpdateLookup(updatelookup);
			insertUpdateMeta.setUpdateStream(updateStream);
			insertUpdateMeta.setUpdate(updateOrNot);

			StepMeta output = new StepMeta("target-table-data-output", insertUpdateMeta);
			transMeta.addStep(output);

			transMeta.addTransHop(new TransHopMeta(input, output));

			Trans trans = new Trans(transMeta);
			trans.execute(null);
			trans.waitUntilFinished();
			if (0 == trans.getErrors()) {
				return true;
			}
		} catch (KettleException e) {
			logger.error("extract table data error:" + e.getMessage());
			throw e;
		}

		return false;
	}

	public boolean exctractTableData(String sourceTableName, String targetTableName, boolean truncateTable)
			throws KettleException {
		try {
			KettleEnvironment.init();
			TransMeta transMeta = new TransMeta();
			transMeta.setName("trans_work");

			DatabaseMeta db_src = new DatabaseMeta("sourceDatabase",
					this.jdbcSourceConnector.getDbType().name().toLowerCase(), "Native",
					this.jdbcSourceConnector.getHostAddress(), this.jdbcSourceConnector.getDbName(),
					String.valueOf(this.jdbcSourceConnector.getServerPort()), this.jdbcSourceConnector.getUserName(),
					this.jdbcSourceConnector.getPassword());

			DatabaseMeta db_dst = new DatabaseMeta("targetDatabase",
					this.jdbcTargetConnector.getDbType().name().toLowerCase(), "Native",
					this.jdbcTargetConnector.getHostAddress(), this.jdbcTargetConnector.getDbName(),
					String.valueOf(this.jdbcTargetConnector.getServerPort()), this.jdbcTargetConnector.getUserName(),
					this.jdbcTargetConnector.getPassword());

			db_dst.addExtraOption(db_dst.getPluginId(), "characterEncoding", "utf8");
			db_src.addExtraOption(db_src.getPluginId(), "zeroDateTimeBehavior", "convertToNull");

			transMeta.addDatabase(db_src);
			transMeta.addDatabase(db_dst);

			TableInputMeta t_input = new TableInputMeta();
			t_input.setDatabaseMeta(transMeta.findDatabase("sourceDatabase"));
			t_input.setSQL(String.format("select * from %s", sourceTableName));
			StepMeta input = new StepMeta("source-table-data-inpurt", t_input);
			transMeta.addStep(input);

			TableOutputMeta t_output = new TableOutputMeta();
			t_output.setDatabaseMeta(transMeta.findDatabase("targetDatabase"));
			t_output.setTableName(targetTableName);
			t_output.setCommitSize(10000);
			t_output.setUseBatchUpdate(true);
			t_output.setTruncateTable(truncateTable);
			StepMeta output = new StepMeta("target-table-data-output", t_output);
			transMeta.addStep(output);
			transMeta.addTransHop(new TransHopMeta(input, output));

			Trans trans = new Trans(transMeta);
			trans.execute(null);
			trans.waitUntilFinished();
			if (0 == trans.getErrors()) {
				return true;
			}
		} catch (KettleException e) {
			logger.error("extract table data error:" + e.getMessage());
			throw e;
		}

		return false;
	}
}
