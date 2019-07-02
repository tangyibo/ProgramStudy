package com.weishao.dmtool.core;

public enum DatabaseType {
	UNKOWN(0), MYSQL(1), ORACLE(2), SQLSERVER(3);

	private int index;

	DatabaseType(int idx) {
		this.index = idx;
	}

	public int getIndex() {
		return index;
	}
}
