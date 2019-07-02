package com.weishao.dmtool.core.except;

public class UnsupportJdbcTypeMapper extends Exception {

	private static final long serialVersionUID = 6747559521438845135L;

    public UnsupportJdbcTypeMapper(String message) {
        super(message);
    }

    public UnsupportJdbcTypeMapper(String message, Throwable cause) {
        super(message, cause);
    }
}
