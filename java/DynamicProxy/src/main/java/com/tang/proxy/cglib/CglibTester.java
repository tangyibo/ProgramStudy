package com.tang.proxy.cglib;

import java.lang.reflect.Method;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

abstract class AbstractSubject {
	private String name;

	public AbstractSubject(String name) {
		this.name = name;
	}

	public final String getName() {
		return this.name;
	}

	public abstract void print();
}

class MySubjectImpl extends AbstractSubject {

	public MySubjectImpl(String name) {
		super(name);
	}

	@Override
	public void print() {
		System.out.println("hello world!,name=" + this.getName());
	}

}

class MyInvokeHandler implements MethodInterceptor {

	@Override
	public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {
		this.before(method);
		Object result = proxy.invokeSuper(obj, args);
		this.after(method);
		return result;
	}

	private void before(Method method) {
		System.out.println("run before method:" + method.getName());
	}

	private void after(Method method) {
		System.out.println("run after method:" + method.getName());
	}
}

public class CglibTester {

	public static void main(String[] args) {
		MyInvokeHandler handler = new MyInvokeHandler();
		CglibProxyUtil cglib = new CglibProxyUtil(handler);
		Class<?>[] argsType = new Class<?>[] { String.class };
		Object[] argsValue = new Object[] { "aa" };
		AbstractSubject proxy = cglib.getInstance(MySubjectImpl.class, argsType, argsValue);
		proxy.print();
	}
}
