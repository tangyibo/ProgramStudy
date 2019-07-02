package com.weishao.dmtool.core.factory;

import java.util.HashMap;
import java.util.Map;
import java.sql.JDBCType;

import com.weishao.dmtool.core.TableColumnDesc;
import com.weishao.dmtool.core.except.UnsupportJdbcTypeMapper;

interface IHandleJdbcType{
	public String handle(TableColumnDesc field);
}

class DefaultTypeHandler implements IHandleJdbcType{

	@Override
	public String handle(TableColumnDesc field) {
		return field.getFieldTypeName().toUpperCase();
	}
}

class DefaultNumberTypeHandler implements IHandleJdbcType{

	@Override
	public String handle(TableColumnDesc field) {
		String name=field.getFieldTypeName();
		int len=field.getDisplaySize();
		return String.format("%s(%d)", name,len);
	}
}

class DefaultTextTypeHandler implements IHandleJdbcType{

	@Override
	public String handle(TableColumnDesc field) {
		int len=field.getDisplaySize();
		if(len<=0) {
			return "LONGTEXT";
		}else if(len<=65535) {
			return "TEXT";
		}else if(len<=166777215) {
			return "MEDIUMTEXT";
		}else {
			return "LONGTEXT";
		}
	}
	
}

public class JdbcTypeMapFactory {
	
	private static final Map<String,String> fieldTypeMapper=new HashMap<String,String>();
	private static final Map<String, IHandleJdbcType> jdbcTypeMapper=new HashMap<String, IHandleJdbcType>();

	static {
		fieldTypeMapper.put("DATETIME", "TIMESTAMP");
		fieldTypeMapper.put("NTEXT", "TEXT");
		fieldTypeMapper.put("VARCHAR2", "VARCHAR");
		fieldTypeMapper.put("NVARCHAR2", "VARCHAR");
		fieldTypeMapper.put("NUMBER", "DECIMAL");
		fieldTypeMapper.put("LONG", "INT");
		
		////////////////////////////////////////////////////////////////////
		
		//比特类型
		jdbcTypeMapper.put(JDBCType.BIT.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				int len=field.getDisplaySize();
				if(len>0 && len<32) {
					return String.format("BIT(%d)", len);
				}
				
				return "INT(11)";
			}
			
		});
		
		//整型
		jdbcTypeMapper.put(JDBCType.TINYINT.getName(),new DefaultNumberTypeHandler());
		jdbcTypeMapper.put(JDBCType.SMALLINT.getName(),new DefaultNumberTypeHandler());
		jdbcTypeMapper.put(JDBCType.INTEGER.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				int len=field.getDisplaySize();
				return String.format("INT(%d)", len);
			}
		});
		jdbcTypeMapper.put(JDBCType.BIGINT.getName(),new DefaultNumberTypeHandler());
		jdbcTypeMapper.put("INT",new DefaultNumberTypeHandler());
		jdbcTypeMapper.put("MEDIUMINT",new DefaultNumberTypeHandler());
		
		//浮点型
		jdbcTypeMapper.put(JDBCType.FLOAT.getName(),new DefaultTypeHandler());
		jdbcTypeMapper.put(JDBCType.REAL.getName(),new IHandleJdbcType () {
			
			@Override
			public String handle(TableColumnDesc field) {
				return "DOUBLE";
			}
		});
		jdbcTypeMapper.put(JDBCType.DOUBLE.getName(),new DefaultTypeHandler());
		jdbcTypeMapper.put(JDBCType.NUMERIC.getName(),new IHandleJdbcType () {
			
			@Override
			public String handle(TableColumnDesc field) {
				String name=field.getFieldTypeName();
				int len=field.getDisplaySize();
				if(len<65) {
					return String.format("%s(%d)", name.toUpperCase(),len);
				}else {
					return "BIGINT";
				}
			}
		});
		jdbcTypeMapper.put(JDBCType.DECIMAL.getName(),new IHandleJdbcType() {
			
			@Override
			public String handle(TableColumnDesc field) {
				return "BIGINT";
//				String name="DECIMAL";	
//				int len=field.getDisplaySize();
//				if(len>30) {
//					len=30;
//				}
//				int pre=field.getPrecisionSize();
//				if(pre>0 && pre>len) {
//					pre=len;
//				}
//				
//				return String.format("%s(%d,%d)", name.toUpperCase(),len,pre);
			}
		});
		
		//字符串类型
		jdbcTypeMapper.put(JDBCType.CHAR.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				String name=field.getFieldTypeName();
				int len=field.getDisplaySize();
				return String.format("%s(%d)",name, len);
			}
		});
		jdbcTypeMapper.put(JDBCType.VARCHAR.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				String name=field.getFieldTypeName();
				int len=field.getDisplaySize();
				if(len<=255) {
					return String.format("%s(%d)",name.toUpperCase(),len);
				}
				
				return "TINYTEXT";
			}
		});
		jdbcTypeMapper.put(JDBCType.LONGVARCHAR.getName(),new DefaultTextTypeHandler());
		jdbcTypeMapper.put("TEXT",new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "TEXT";
			}
		});
		
		//日期时间类型
		jdbcTypeMapper.put(JDBCType.DATE.getName(),new DefaultTypeHandler());
		jdbcTypeMapper.put(JDBCType.TIME.getName(),new DefaultTypeHandler());
		jdbcTypeMapper.put(JDBCType.TIMESTAMP.getName(),new DefaultTypeHandler());
		jdbcTypeMapper.put("YEAR",new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "INT(11)";
			}
		});
		
		//块类型
		jdbcTypeMapper.put(JDBCType.BINARY.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "BLOB";
			}
		});
		jdbcTypeMapper.put(JDBCType.VARBINARY.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				String name=field.getFieldTypeName();
				int len=field.getDisplaySize();
				return String.format("%s(%d)",name, len);
			}
		});
		jdbcTypeMapper.put(JDBCType.LONGVARBINARY.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "LONGBLOB";
			}
		});
		jdbcTypeMapper.put("TINYBLOB",new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "BLOB";
			}
		});
		jdbcTypeMapper.put("LONGBLOB",new DefaultTypeHandler());
		jdbcTypeMapper.put("MEDIUMBLOB",new DefaultTypeHandler());
		
		//杂项类型
		jdbcTypeMapper.put(JDBCType.NULL.getName(),null);
		jdbcTypeMapper.put(JDBCType.OTHER.getName(),null);
		jdbcTypeMapper.put(JDBCType.JAVA_OBJECT.getName(),null);
		jdbcTypeMapper.put(JDBCType.DISTINCT.getName(),null);
		jdbcTypeMapper.put(JDBCType.STRUCT.getName(),null);
		jdbcTypeMapper.put(JDBCType.ARRAY.getName(),null);
		
		//二进制类型
		jdbcTypeMapper.put(JDBCType.BLOB.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "LONGBLOB";
			}
		});
		
		//杂项类型
		jdbcTypeMapper.put(JDBCType.CLOB.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "TEXT";
			}
		});
		jdbcTypeMapper.put(JDBCType.REF.getName(),null);
		jdbcTypeMapper.put(JDBCType.DATALINK.getName(),null);
		
		//布尔类型
		jdbcTypeMapper.put(JDBCType.BOOLEAN.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "INT(11)";
			}
		});
		
		//杂项类型
		jdbcTypeMapper.put(JDBCType.ROWID.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
					return "CHAR(18)";
			}
		});
		jdbcTypeMapper.put(JDBCType.NCHAR.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				int len=field.getDisplaySize();
				if(len<128) {
					return String.format("VARCHAR(%d)",2*len);
				}else {
					return "TEXT";
				}
			}
		});
		jdbcTypeMapper.put(JDBCType.NVARCHAR.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				int len=field.getDisplaySize();
				if(len<=255) {
					return String.format("VARCHAR(%d)",len);
				}else {
					return "TEXT";
				}
			}
		});
		jdbcTypeMapper.put(JDBCType.LONGNVARCHAR.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				int len=field.getDisplaySize();
				if(len<128) {
					return String.format("VARCHAR(%d)",2*len);
				}else {
					return "LONGTEXT";
				}
			}
		});
		jdbcTypeMapper.put(JDBCType.NCLOB.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "LONGBLOB";
			}
		});
		jdbcTypeMapper.put(JDBCType.SQLXML.getName(),new DefaultTextTypeHandler());
		jdbcTypeMapper.put(JDBCType.REF_CURSOR.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "VARCHAR(255)";
			}
		});
		jdbcTypeMapper.put(JDBCType.TIME_WITH_TIMEZONE.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "VARCHAR(255)";
			}
		});
		jdbcTypeMapper.put(JDBCType.TIMESTAMP_WITH_TIMEZONE.getName(),new IHandleJdbcType() {

			@Override
			public String handle(TableColumnDesc field) {
				return "VARCHAR(255)";
			}
		});
	}
	
	public static String getMySQLFieldTypeFromJdbcType(TableColumnDesc field) throws UnsupportJdbcTypeMapper {

		if(fieldTypeMapper.containsKey(field.getFieldTypeName().toUpperCase())) {
			field.setFieldTypeName(fieldTypeMapper.get(field.getFieldTypeName().toUpperCase()));
		}
		
		if(field.getFieldTypeName().contains(" ")) {
			String fieldJdbcType=field.getFieldTypeName().substring(0, field.getFieldTypeName().indexOf(" "));
			field.setFieldTypeName(fieldJdbcType);
		}
		if(jdbcTypeMapper.containsKey(field.getFieldTypeName().toUpperCase())) {
			IHandleJdbcType handler=jdbcTypeMapper.get(field.getFieldTypeName().toUpperCase());
			if(null!=handler) {
				return handler.handle(field);
			}
		}
		
		throw new UnsupportJdbcTypeMapper(String.format("unsupport jdbc type convert for JdbcType=%s",field.getFieldTypeName()));
	}
}
