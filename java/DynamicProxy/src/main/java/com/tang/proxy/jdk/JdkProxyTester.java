package com.tang.proxy.jdk;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

interface ISubject {
	public void print();
}

class SubjectImpl implements ISubject {

	@Override
	public void print() {
		System.out.println("hello world!");
	}

}

class MyInvokeHandler implements InvocationHandler {
	
	private Object instance;
	
	public MyInvokeHandler(Object o) {
		this.instance=o;
	}
	
	@Override
	public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
		this.before(method);
		Object result=method.invoke(this.instance, args);
		this.after(method);
		return result;
	}

	private void before(Method method) {
		System.out.println("run before method:"+method.getName());
	}
	
	private void after(Method method) {
		System.out.println("run after method:"+method.getName());
	}
}

public class JdkProxyTester {

	public static void main(String[] args) {
		ISubject subject=new SubjectImpl();
		MyInvokeHandler handler=new MyInvokeHandler(subject);
		JdkProxyUtil jdk=new JdkProxyUtil(handler);
		ISubject proxy=jdk.getInstance(ISubject.class);
		proxy.print();

	}

}
