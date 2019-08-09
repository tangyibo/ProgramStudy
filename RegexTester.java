package com.weishao.test;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegexTester {

	public static void main(String[] args) {
		usage() ;
	}
	
	public static void test() {
		String str="userservice:v5";
        Pattern pattern=Pattern.compile("(?<name>^.+):(?<version>v.+$)");
        Matcher m=pattern.matcher(str);
        if (m.find()){
            System.out.println("name:"+m.group("name"));
            System.out.println("version:"+m.group("version"));
        }else {
        	System.out.println("match failed!");
        }
	}

	public static void usage() {
		String str="jdbc:mysql://172.16.90.119:3306/whistle?autoReconnect=true&useUnicode=true&characterEncoding=utf-8&useSSL=true";
		//String str="jdbc:mysql://172.16.90.119:3306/whistle";
		Pattern pattern=null;
		if(str.indexOf('?')>0) {
			pattern=Pattern.compile("(?<protocol>^.+):(?<dbtype>.+)://(?<addresss>.+):(?<port>.+)/(?<schema>.+)\\?(?<path>.+)");
		}else {
			pattern=Pattern.compile("(?<protocol>^.+):(?<dbtype>.+)://(?<addresss>.+):(?<port>.+)/(?<schema>.+)");
		}
		
        Matcher m=pattern.matcher(str);
        if (m.find()){           
        	System.out.println("dbtype:"+m.group("protocol"));
            System.out.println("dbtype:"+m.group("dbtype"));
            System.out.println("addresss:"+m.group("addresss"));
            System.out.println("port:"+m.group("port"));
            System.out.println("schema:"+m.group("schema"));
            if(m.groupCount()>5) {
            	System.out.println("params:"+m.group("path"));
            }
        }else {
        	System.out.println("match failed!");
        }
	}
}
