package com.learn.test;

import com.learn.util.ConfigFile;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import java.util.Properties;
import java.util.Arrays;

import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;


public class TestKafkaConsumer 
{
	private static Logger logger = Logger.getLogger(TestKafkaConsumer.class.getName());

	public static void main(String[] args) 
	{
		PropertyConfigurator.configure("conf/log4j.properties");
		logger.info("start ....");
	
		Properties props = new Properties();  
        	props.put("bootstrap.servers", ConfigFile.instance().getKafkaBrokers());
		props.put("group.id", ConfigFile.instance().getKafkaGroupName());
		props.put("auto.offset.reset", "latest");
        	props.put("enable.auto.commit", "true");
        	props.put("auto.commit.interval.ms", "1000");
        	props.put("session.timeout.ms", "30000");
		props.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
		props.put("value.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");

		KafkaConsumer<String, String> consumer = new KafkaConsumer<String, String>(props);
		String topic=ConfigFile.instance().getKafkaTopic();
		logger.info("Topic name is :"+topic);
		logger.info("Broker list is :"+ConfigFile.instance().getKafkaBrokers());
		logger.info("Group name is :"+ConfigFile.instance().getKafkaGroupName());
		consumer.subscribe(Arrays.asList(topic));
	
		while(true)
		{
			logger.info("Prepare poll message from topic :"+topic);
			ConsumerRecords<String, String> records = consumer.poll(100);
			if(records.count() > 0){
				for (ConsumerRecord<String, String> record : records) {
					 System.out.printf("partition = %d, offset = %d, key = %s,value = %s\n", 
						record.partition(), record.offset(),record.key(), record.value());
					logger.info(record.value());
				}
			}

			logger.info("Fetch kafka message count :"+records.count());
		}

		//consumer.close();
		//logger.info("exit ....");
	}
	
}
