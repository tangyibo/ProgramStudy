package com.weishao.dmtool.core.except;

public class UnsupportDatabaseTypeException extends Exception {

	private static final long serialVersionUID = 6747559501498845135L;

    public UnsupportDatabaseTypeException(String message) {
        super(message);
    }

    public UnsupportDatabaseTypeException(String message, Throwable cause) {
        super(message, cause);
    }
}
