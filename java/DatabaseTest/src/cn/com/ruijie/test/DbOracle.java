package cn.com.ruijie.test;

import cn.com.ruijie.dbutil.DbConnector;
import java.io.File;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import org.json.JSONArray;
import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 *
 * @author Administrator
 */
public class DbOracle extends DbBase {

    private static final Logger logger = LoggerFactory.getLogger(DbOracle.class);

    public static void main(String[] args) {
        String logConfig = args[0] + File.separator + "log4j.properties";
        String configFile = args[0] + File.separator + "config.properties";

        PropertyConfigurator.configure(logConfig);

        logger.info("Configuer for log4j:" + logConfig);
        logger.info("Configuer for config:" + configFile);
 
        String type = "oracle";
        String dbname = "urpdb";
        String host = "127.0.0.1";
        String port = "1521";
        String username = "usr_ws";
        String password = "db_ws";
        String mode = "ServiceName";
        Connection conn = DbConnector.open(type, dbname, host, port, username, password, mode);
        if (null != conn) {
            String tablename = "WS_USER";
            String sql = String.format("select* from %s", tablename);

            PreparedStatement pstmt = null;
            ResultSet rs = null;
            try {
                pstmt = conn.prepareStatement(sql);
                rs = pstmt.executeQuery();
                JSONArray ret = resultToJsonArray(rs);
                if (ret != null) {
                    //System.out.println(ret.toString());
                    writeFile("~/oracle_test.txt", ret.toString());
                }

            } catch (SQLException e) {
                logger.error("error:" + e.getMessage());
            } finally {
                DbConnector.close(conn, pstmt, rs);
            }
        } else {
            logger.error("Connect Oracle Failed!");
        }
    }
}
