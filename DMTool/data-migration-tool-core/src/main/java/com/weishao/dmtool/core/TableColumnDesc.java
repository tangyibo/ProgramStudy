package com.weishao.dmtool.core;

public class TableColumnDesc {
	
	private String fieldName;
	private String labalName;
	private String fieldTypeName;
	private String filedTypeClassName;
	private int fieldType;
	private int displaySize;
	private int scaleSize;
	private int precisionSize;
	private boolean isAutoIncrement;
	private boolean isNullable;
	
	public String getFieldName() {
		return fieldName;
	}
	public void setFieldName(String fieldName) {
		this.fieldName = fieldName;
	}
	public String getLabalName() {
		return labalName;
	}
	public void setLabalName(String labalName) {
		this.labalName = labalName;
	}
	public String getFieldTypeName() {
		return fieldTypeName;
	}
	public void setFieldTypeName(String fieldTypeName) {
		this.fieldTypeName = fieldTypeName;
	}
	public String getFiledTypeClassName() {
		return filedTypeClassName;
	}
	public void setFiledTypeClassName(String filedTypeClassName) {
		this.filedTypeClassName=filedTypeClassName;
	}
	public int getFieldType() {
		return fieldType;
	}
	public void setFieldType(int fieldType) {
		this.fieldType = fieldType;
	}
	public int getDisplaySize() {
		return displaySize;
	}
	public void setDisplaySize(int displaySize) {
		this.displaySize = displaySize;
	}
	public int getScaleSize() {
		return scaleSize;
	}
	public void setScaleSize(int scaleSize) {
		this.scaleSize = scaleSize;
	}
	public int getPrecisionSize() {
		return precisionSize;
	}
	public void setPrecisionSize(int precisionSize) {
		this.precisionSize = precisionSize;
	}
	public boolean isAutoIncrement() {
		return isAutoIncrement;
	}
	public void setAutoIncrement(boolean isAutoIncrement) {
		this.isAutoIncrement = isAutoIncrement;
	}
	public boolean isNullable() {
		return isNullable;
	}
	public void setNullable(boolean isNullable) {
		this.isNullable = isNullable;
	}
	
	
}
