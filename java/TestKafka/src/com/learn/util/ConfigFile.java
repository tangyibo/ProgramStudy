package com.learn.util;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;

import org.apache.log4j.Logger;

public class ConfigFile{
	private static Logger logger = Logger.getLogger(ConfigFile.class.getName());

	private static String FILE_NAME = "conf/config.properties";
	private static ConfigFile config = null;

	private String kafkabrokers;
	private String kafkagroupname;
	private String kafkatopic;
	
	public static String getConfigFile() {
		return FILE_NAME;
	}

	public static void setConfigFile(String file) {
		FILE_NAME = file;
	}

	public static ConfigFile instance() {
		if (config == null) {
			config = new ConfigFile();
			config.ReadConfig();
		}
		return config;
	}

	public String getKafkaBrokers() {
		return this.kafkabrokers;
	}

	public String getKafkaGroupName() {
		return this.kafkagroupname;
	}
	
	public String getKafkaTopic() {
		return this.kafkatopic;	
	}

	private void ReadConfig() {
		Properties rb = new Properties();
		try {	
			logger.info("Load config file ["+FILE_NAME+"]...");
			rb.load(new FileInputStream(FILE_NAME));

			kafkabrokers= rb.getProperty("kafka.brokers").trim();
			kafkagroupname = rb.getProperty("kafka.group").trim();
			kafkatopic=rb.getProperty("kafka.topic").trim();
		}catch (FileNotFoundException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		} catch (IOException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
	}

	public static void main(String[] args) {
		System.out.println(ConfigFile.instance().getKafkaTopic());
	}	
}
