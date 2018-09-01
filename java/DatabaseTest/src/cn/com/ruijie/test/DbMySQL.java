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
public class DbMySQL extends DbBase {

    private static final Logger logger = LoggerFactory.getLogger(DbMySQL.class);

    public static void main(String[] args) {
	String path=args[0];	 
	if(path.endsWith(File.separator)){
	   path=path.substring(0, path.length()-1);
	}
	
	System.err.println("Read args count="+args.length);       
 
	String logConfig = path+ File.separator + "log4j.properties";
        String configFile = path + File.separator + "config.properties";
        
        PropertyConfigurator.configure(logConfig);
        logger.info("Configuer for log4j:"+logConfig);
        logger.info("Configuer for config:"+configFile);

        String type = "mysql";
        String dbname = "data_sync";
        String host = "127.0.0.1";
        String port = "3306";
        String username = "root";
        String password = "";
        String mode = "";
        Connection conn = DbConnector.open(type, dbname, host, port, username, password, mode);
        if (null != conn) {
            String tablename = "organization";
            String sql = String.format("select id,name from %s", tablename);

            PreparedStatement pstmt = null;
            ResultSet rs = null;
            try {
                pstmt = conn.prepareStatement(sql);
                rs = pstmt.executeQuery();
                JSONArray ret = resultToJsonArray(rs);
                if (ret != null) {
                    //System.out.println(ret.toString());
                    writeFile("~/mysql_test.txt", ret.toString());
                }
            } catch (SQLException e) {
                logger.error("Error:" + e.getMessage());
            } finally {
                DbConnector.close(conn, pstmt, rs);
            }
        } else {
            logger.error("Connect to MySQL failed!");
        }
    } 
}
