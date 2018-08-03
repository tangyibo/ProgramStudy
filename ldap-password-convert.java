package com.test;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Base64;

public class Test {

	public static void main(String[] args) {
		//shell 命令：slappasswd -h {md5} -s "123456"
		//PHP实现：
		//<?php
		//		$passwd = "123456";
		//		$md5hash = base64_encode(pack( 'H*',md5("$passwd")));
		//		printf("%s\n",$md5hash); 
		//		
		try {
			String password = "123456";
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(password.getBytes());
			byte[] bs = md.digest();
			Base64.Encoder encoder = Base64.getEncoder();
			byte[] base64MD5Password = encoder.encode(bs);
			System.out.println("base64MD5Password:{MD5}" + new String(base64MD5Password));
		} catch (NoSuchAlgorithmException e) {

		}
	}

}
