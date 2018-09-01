package com.learn.test;

import com.learn.util.ConfigFile;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import java.util.Properties;
import java.util.Arrays;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;

public class TestKafkaProducer 
{
	private static Logger logger = Logger.getLogger(TestKafkaProducer.class.getName());

	public static void main(String[] args) 
	{
		PropertyConfigurator.configure("conf/log4j.properties");
		logger.info("start ....");
	
		Properties props = new Properties();  
        	props.put("bootstrap.servers", ConfigFile.instance().getKafkaBrokers());
        	props.put("acks", "all");
        	props.put("retries", 0);
        	props.put("batch.size", 16384);
        	props.put("linger.ms", 1);
        	props.put("buffer.memory", 33554432);
        	props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        	props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");

		Producer<String, String> producer = new KafkaProducer<String, String>(props);
		String topic=ConfigFile.instance().getKafkaTopic();
		logger.info("Topic name is :"+topic);
		logger.info("Broker list is :"+ConfigFile.instance().getKafkaBrokers());
	
		int i=0;
		while(true)
		{
			String msg="hello "+Integer.toString(++i);
			producer.send(new ProducerRecord<String, String>(topic, msg, msg));

			logger.info("Produce message for topic["+topic+"] :"+msg);
		}

		//producer.close();
		//logger.info("exit ....");
	}
	
}
