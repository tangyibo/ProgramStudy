package cn.com.ruijie.dbutil;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 *
 * @author Administrator
 */
public class DbConnector {
    
     private static final Logger logger = LoggerFactory.getLogger(DbConnector.class);
    
    /**
     *  Description:建立与数据库间到连接
     * 
     * @param type            ,String 数据库类型，当前支持mysql,oracle,sqlserver
     * @param dbname      ,String 数据库库名称
     * @param host            ,String 主机IP地址
     * @param port            ,String 端口号
     * @param username   ,String 登录的用户名
     * @param password   ,String 登录的密码
     * @param mode        ,对于Oracle数据库，传递ServiceName/SID/TNSName等额外参数
     * @return       java.sql.Connection ,连接对象
     */
    public static Connection open(String type,String dbname, String host, String port, String username, String password,String mode){

        Connection conn=null;
        String url = null;
       
        try {
            switch (type.toLowerCase()) {
                case "mysql":
                    url = "jdbc:mysql://" + host +":"+port+ String.format("/%s?useUnicode=true&characterEncoding=utf8&zeroDateTimeBehavior=convertToNull", dbname);
                    Class.forName("com.mysql.jdbc.Driver");//要求JVM查找并加载指定的类，也就是说JVM会执行该类的静态代码段
                    break;
                case "oracle":
                    if (mode.equals("ServiceName")) {
                        url = "jdbc:oracle:thin:@//" + host + ":" + port + "/" + dbname;
                    } else if (mode.equals("SID")) {
                        url = "jdbc:oracle:thin:@" + host + ":" + port + ":" + dbname;
                    } else if (mode.equals("TNSName")) {
                        url = "jdbc:oracle:thin:@" + dbname;
                    } else {
                        //Log.debug("mode error. type is:" + mode);
                    }   Class.forName("oracle.jdbc.driver.OracleDriver");//要求JVM查找并加载指定的类，也就是说JVM会执行该类的静态代码段
                    break;
                case "sqlserver":
                    url = "jdbc:sqlserver://" + host + ":" + port + ";" + "DatabaseName=" + dbname;
                    Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");//要求JVM查找并加载指定的类，也就是说JVM会执行该类的静态代码段
                    break;
                default:
                    return null;
            }
            
            logger.debug("Database Connect uri:"+url);
            conn = DriverManager.getConnection(url, username, password);
        } catch (ClassNotFoundException | SQLException e) {
            logger.error("Connection::Open() Error:"+e.getMessage());
            return null;
        }
        
        return conn;
    }

    /**
     *  Description:断开与数据库间到连接
     * 
     * @param con
     * @param stmt
     * @param rs
     */
    public static void close(Connection con, Statement stmt, ResultSet rs) {
        try {
            if (rs != null) {
                rs.close();
            }
        } catch (SQLException e) {
            logger.debug("Close ResultSet Exception:" + e);
        }

        try {
            if (stmt != null) {
                stmt.close();
            }
        } catch (SQLException e) {
            logger.debug("Close PreparedStatement Exception:" + e);
        }

        try {
            if (con != null) {
                con.close();
            }
        } catch (SQLException e) {
            logger.debug("Close Connection Exception:" + e);
        }
    }
    
    
}
