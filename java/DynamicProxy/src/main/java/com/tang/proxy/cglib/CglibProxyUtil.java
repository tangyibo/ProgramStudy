package com.tang.proxy.cglib;

import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;

/**
 * CGLib动态代理封装工具类
 * 
 * @author tang
 *
 */
public class CglibProxyUtil {
	
	private MethodInterceptor handler;
	
	public CglibProxyUtil(MethodInterceptor handler) {
		this.handler=handler;
	}

	public <T> T getInstance(Class<T> clazz, Class[] args, Object[] argsValue) {
		Enhancer enhancer = new Enhancer();
		enhancer.setSuperclass(clazz);
		enhancer.setCallback(this.handler);
		return (T) enhancer.create(args, argsValue);
	}

	public <T> T getInstance(Class<T> clazz) {
		Enhancer enhancer = new Enhancer();
		enhancer.setSuperclass(clazz);
		enhancer.setCallback(this.handler);
		return (T) enhancer.create();
	}
}
