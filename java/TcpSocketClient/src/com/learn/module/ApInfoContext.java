package com.learn.module;

import java.io.Serializable; 

public class ApInfoContext implements Serializable {
        public byte[] ap_mac=new byte[6];
        public byte ap_rssi;
        public byte ap_is_associated;

        public ApInfoContext(String mac,byte rssi,byte is_associated){
                String [] strArr = mac.split(":");

                for(int i = 0;i < strArr.length; i++){
                        int value = Integer.parseInt(strArr[i],16);
                        this.ap_mac[i] = (byte) value;
                }     
                this.ap_rssi=rssi;
                this.ap_is_associated=is_associated;
        }
}
