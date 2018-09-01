package com.learn.module;  

import java.io.*;
import java.io.Serializable;
import java.util.List;
import java.util.ArrayList;

public class LocationMessage implements Serializable{  
        //header
        private byte[] header={ (byte)0x7b, (byte)0x81};           // 2 bytes
        private short length=51;                        // 2 bytes

        //body
        private int map_id;                          // 4 bytes
        private int area_id;                         // 4 bytes
        private int poi_id;                          // 4 bytes
        private byte[] mu_mac_address=new byte[6];                        // 6 bytes
        private long timestamp=System.currentTimeMillis();                // 8 bytes
        private float x;
	private float y;
	private float z;
        private byte is_associated;                                       // 1 bytes
	private ArrayList<ApInfoContext> options=new ArrayList<ApInfoContext>();

        public void setHeaderLenght(short len ){
		this.length=len;
        }

        public void setMapId(int mapid){
                this.map_id=mapid;
        }

        public void setAreaId(int areaid){
                this.area_id=areaid;
        }

        public void setPoiId(int poiid){
                this.poi_id=poiid;
        }

        public void setMuMacAddress(String mac){
                String [] strArr = mac.split(":");

                for(int i = 0;i < strArr.length; i++){
                        int value = Integer.parseInt(strArr[i],16);
                        this.mu_mac_address[i] = (byte) value;
                }
        }

        public void setCoordinate(float x,float y,float z)
        {
                this.x=x;
		this.y=y;
		this.z=z;
        }

        public void setIsAssociated(byte is_associated)
        {
                this.is_associated=is_associated;
        }

        public void AddApOptions(ApInfoContext ap)
        {
                options.add(ap);
        }

	public byte[] toBytesArray()
	{
		try{
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			ObjectOutputStream out = new ObjectOutputStream(baos);
			out.writeObject(this.header);
			out.writeObject(this.length);
			out.writeObject(this.map_id);
			out.writeObject(this.area_id);
			out.writeObject(this.poi_id);
			out.writeObject(this.mu_mac_address);
			out.writeObject(this.timestamp);
			out.writeObject(this.x);
			out.writeObject(this.y);
			out.writeObject(this.z);
			out.writeObject(this.is_associated);
			for(int i=0;i<this.options.size();i++){
				ApInfoContext ap=this.options.get(i);
				out.writeObject(ap.ap_mac);
				out.writeObject(ap.ap_rssi);
				out.writeObject(ap.ap_is_associated);
			}

			return baos.toByteArray();
		} catch (IOException e){
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
} // end   
