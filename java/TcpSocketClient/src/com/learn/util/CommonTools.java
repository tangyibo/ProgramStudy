package com.learn.util;

public class CommonTools{

    /** 
     * 将int转为低字节在前，高字节在后的byte数组 
     */  
    public static byte[] tolh(int n) {  
        byte[] b = new byte[4];  
        b[0] = (byte) (n & 0xff);  
        b[1] = (byte) (n >> 8 & 0xff);  
        b[2] = (byte) (n >> 16 & 0xff);  
        b[3] = (byte) (n >> 24 & 0xff);  
        return b;  
    }
	
	
    /** 
     * 将float转为低字节在前，高字节在后的byte数组 
     */  
    private static byte[] tolh(float f) {  
        return tolh(Float.floatToRawIntBits(f));  
    }  
      
	  
    /** 
     * 将byte数组转化成String 
     */  
    public static String toStr(byte[] valArr,int maxLen) {  
        int index = 0;  
        while(index < valArr.length && index < maxLen) {  
            if(valArr[index] == 0) {  
                break;  
            }  
            index++;  
        }  
        byte[] temp = new byte[index];  
        System.arraycopy(valArr, 0, temp, 0, index);  
        return new String(temp);  
    }  
      
    /** 
     * 将低字节在前转为int，高字节在后的byte数组 
     */  
    public static int vtolh(byte[] bArr) {  
        int n = 0;  
        for(int i=0;i<bArr.length&&i<4;i++){  
            int left = i*8;  
            n+= (bArr[i] << left);  
        }  
        return n;  
    } 
	
}
