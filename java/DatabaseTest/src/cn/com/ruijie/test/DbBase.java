package cn.com.ruijie.test;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 *
 * @author Administrator
 */
public class DbBase {

    private static final Logger logger = LoggerFactory.getLogger(DbBase.class);

    public static JSONArray resultToJsonArray(ResultSet rs) {
        if (rs == null) {
            return null;
        }

        JSONArray ret = new JSONArray();
        try {
            while (rs.next()) {
                ResultSetMetaData metaData = rs.getMetaData();
                JSONObject result = new JSONObject();
                for (int j = 1; j <= metaData.getColumnCount(); ++j) {
                    String key = metaData.getColumnName(j).toLowerCase();
                    String value = rs.getString(key);
                    if (value == null) {
                        value = "";
                    }
                    result.put(key, value.trim());
                }
                ret.put(result);
            }

        } catch (SQLException | JSONException e) {
            logger.error("Error:"+e);
            return null;
        }

        return ret;
    }

    public static void writeFile(String filename, String data) {
        try {
            File f = new File(filename);
            try (Writer wt = new FileWriter(f)) {
                wt.write(data);
            }
        } catch (IOException e) {
            logger.error("Error:"+e);
        }
    }
}
