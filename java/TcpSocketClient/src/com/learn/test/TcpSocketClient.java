package com.learn.test;

import java.net.*;
import java.io.*;
import java.util.*;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import com.learn.util.ConfigFile;
import com.learn.module.*;

class TcpSocketClient{
        private static Logger logger = Logger.getLogger(TcpSocketClient.class.getName());

        /** 
         * 与c语言通信(java做client，c/c++做server，传送一个结构) 
         *  
         * @author tang 
         * @version 1.0 
         */  

        public static void main(String[] args) {  
                
                PropertyConfigurator.configure("conf/log4j.properties");
                logger.info("start ....");

                try {  
                        int index = 0;  

                        //Socket sock = new Socket("127.0.0.1", 6666);  
                        logger.info("connect to server ...");
                        String testmac=new String("00:13:e4:c4:4c:67");
                        float x=1.1f,y=2.2f,z=3.3f;
                        while(true){
                                LocationMessage msg=new LocationMessage();
                                msg.setMapId(1);
                                msg.setAreaId(2);
                                msg.setPoiId(3);
                                msg.setMuMacAddress(testmac);
                                msg.setCoordinate(x,y,z);
                                msg.setIsAssociated((byte)0x01);
                                msg.AddApOptions(new ApInfoContext(testmac,(byte)1,(byte)1));

				byte[] ObjByte = msg.toBytesArray();
				logger.info("Send bytes size ="+ObjByte.length);
				FileOutputStream fs = new FileOutputStream("./dump.log");
				fs.write(ObjByte,0,ObjByte.length);
                                //sock.getOutputStream().write(baos.toByteArray());  
                                fs.close();

				break;
                        }

                        //sock.close();
                } catch (IOException e){
                        e.printStackTrace();
                } catch (Exception e) {  
                        e.printStackTrace();  
                }  
        }  
}
