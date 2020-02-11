package com.tang.proxy.jdk;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;

/**
 * JDK的动态代理封装工具类
 * 
 * @author tang
 *
 */
public class JdkProxyUtil {

	private InvocationHandler handler;

	public JdkProxyUtil(InvocationHandler handler) {
		this.handler = handler;
	}

	public <T> T getInstance(Class<?>[] interfaces) {
		ClassLoader loader = interfaces[0].getClassLoader();
		Object obj = Proxy.newProxyInstance(loader, interfaces, this.handler);
		return (T) obj;
	}

	public <T> T getInstance(Class<?> interfaceClass) {
		ClassLoader loader = interfaceClass.getClassLoader();
		Class<?>[] interfaces = { interfaceClass };
		return getInstance(interfaces);
	}
}
