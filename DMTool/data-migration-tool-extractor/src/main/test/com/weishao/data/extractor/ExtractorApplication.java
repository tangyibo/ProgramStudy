package com.weishao.data.extractor;

import java.util.HashMap;
import java.util.Map;

import com.weishao.data.extractor.engine.DataMigrationEngine;
import com.weishao.dmtool.DMTool;
import com.weishao.dmtool.core.DatabaseType;
import com.weishao.dmtool.core.JdbcConnector;

public class ExtractorApplication {
	public static void main(String[] args) {
		DMTool dm = new DMTool();
		try {
			JdbcConnector sourceConnector = new JdbcConnector(DatabaseType.ORACLE);
			sourceConnector.setHostAddress("172.16.90.252");
			sourceConnector.setServerPort(1521);
			sourceConnector.setDbName("orcl");
			sourceConnector.setUserName("yi_bo");
			sourceConnector.setPassword("yi_bo");

			JdbcConnector targetConnector = new JdbcConnector(DatabaseType.MYSQL);
			targetConnector.setHostAddress("172.16.90.210");
			targetConnector.setServerPort(3306);
			targetConnector.setDbName("tangyibo");
			targetConnector.setUserName("tangyibo");
			targetConnector.setPassword("tangyibo");

			dm.setSourceDatabase(sourceConnector);

			DataMigrationEngine extractor = new DataMigrationEngine(dm, targetConnector);

			Map<String, String> tableMappers = new HashMap<String, String>();
			tableMappers.put("OFUSER_CAS", "new_OFUSER_CAS");

			extractor.doMigration(tableMappers, true,true);
		} catch (Exception e) {
			e.printStackTrace();
		}finally {
			dm.close();
		}
	}
}
