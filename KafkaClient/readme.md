# KafkaClient封装类


## 简介

- librdkafka 是Apache  Kafka  客户端C语言的高性能实现，这里对该库进行了封装，便于直接使用。
- 包含了对Producer和Consumer的封装类。

## 方法

- 生产者使用

```
CKafkaProducer producer;
if(producer.Init(brokers,topic))
{
	int i=0;	
	while(i<10000)
	{
		producer.Send("hello");			
		sleep(1);			
	}
	
	producer.Close();		
}
```

- 消费者使用

```
CKafkaConsumer consumer;
if(consumer.Init(brokers,topic,"test_tang"))
{
	string data;
	
	while(1)	
	{
            my_kafka_result ret=consumer.Recv(data,1000);  
            if(MY_KAFKA_OPERATOR_OK!=ret) 
            {
                if(MY_KAFKA_RECV_DATA_TIMEOUT!=ret)
                {
                        std::cout<<consumer.GetLastError()<<std::endl;;
                }
                else
                {
                        continue;
                } 
            }

            std::cout<<data<<std::endl;	
	}
	
	consumer.Close();
 }
```
