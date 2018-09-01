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
public class DbSqlserver extends DbBase {

    private static final Logger logger = LoggerFactory.getLogger(DbMySQL.class);

    public static void main(String[] args) {

        String type = "sqlserver";
        String dbname = "edu";
        String host = "172.10.5.63";
        String port = "1433";
        String username = "ei";
        String password = "LSbz800444";
        String mode = "";
        Connection conn = DbConnector.open(type, dbname, host, port, username, password, mode);
        if (null != conn) {
            String tablename = "ws_org";
            String sql = String.format("select * from %s", tablename);

            PreparedStatement pstmt = null;
            ResultSet rs = null;
            try {
                pstmt = conn.prepareStatement(sql);
                rs = pstmt.executeQuery();
                JSONArray ret = resultToJsonArray(rs);
                if (ret != null) {
                    System.out.println(ret.toString());
                }
            } catch (SQLException e) {
                System.out.println("Error:" + e.getMessage());
            } finally {
                DbConnector.close(conn, pstmt, rs);
            }
        } else {
            System.out.println("Connect to SqlServer failed!");
        }
    } 
}
