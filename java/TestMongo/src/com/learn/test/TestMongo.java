package com.learn.test;

import org.json.JSONException;
import org.json.JSONObject;

import org.bson.Document;
import com.mongodb.MongoClientURI;
import com.mongodb.MongoClient;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.MongoCollection;

//import java.util.logging.Logger;
//import java.util.logging.Level;

import com.learn.util.ConfigFile;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class TestMongo 
{
	private static Logger logger = Logger.getLogger(TestMongo.class.getName());

	public static void main(String[] args) throws JSONException 
	{
		PropertyConfigurator.configure("conf/log4j.properties");

		logger.info("Mongo server uri address : "+ConfigFile.instance().getMongouri());

		//follow code will be usefull when do not use log4j as logger				
		//Logger log = Logger.getLogger("org.mongodb.driver");   
          	//log.setLevel(Level.OFF);  
	
		MongoClientURI uri=new MongoClientURI(ConfigFile.instance().getMongouri());
        	MongoClient client=new MongoClient (uri);

        	MongoCursor<String> dbs=client.listDatabaseNames().iterator();
        	System.out.println("mongodb database is :");
        	while(dbs.hasNext())
        	{  
            		System.out.println(dbs.next());  
        	}  
        
        	MongoDatabase db=client.getDatabase(ConfigFile.instance().getMongodb());  
        	MongoCollection<Document> collection=db.getCollection(ConfigFile.instance().getmongoCollection());  
		MongoCursor<Document> cursor=collection.find().iterator();
        	System.out.println("mongodb hotwords-hot_config data is:");
        	while(cursor.hasNext())
        	{  
        		JSONObject obj=new JSONObject(cursor.next());
            		System.out.println("URL:"+obj.getString("url"));  
        	}  
        
        	client.close();
		logger.info("exit program ...");
	}
	
}
