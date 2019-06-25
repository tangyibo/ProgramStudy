/*
 * main.cpp
 *
 *  Created: 2016年9月2日
 *   Author: tang
 */
#include "CKafkaNode.h"
#include <unistd.h>

int main(int argc,char *argv[])
{
	string brokers="192.168.1.2:9200,192.168.1.2:9200";
	string topic="test_doc";

#if 0
	CKafkaConsumer consumer;
	if(consumer.Init(brokers,topic,"fsfdafsdssssssss"))
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

#else
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

#endif

	return 0;
}


