#include <iostream>
#include "RabbitmqCpp.h"
#include <unistd.h>
#include <stdio.h>

int main(int arg,char *argv[])
{
        string host="127.0.0.1";
        int port=5672;
        string user="test";
        string passwd="123456";
        string exchange="ex_name";
        string queue="queue_name";
        string routing_key="routing_key";
        string vhost="private";

        RabbitProducer producer;
        if(!producer.OpenConnect(host,port))
        {
                std::cout<<producer.GetLastError()<<std::endl;
                return 1;
        }

        if(!producer.Login(user,passwd,vhost))
        {
                std::cout<<producer.GetLastError()<<std::endl;
                return 1;
        }

        if(!producer.Init(queue,exchange))
        {
                std::cout<<producer.GetLastError()<<std::endl;
                return 1;
        }

        int i=0;
        while(i<10000)
        {
		char buf[128];
		sprintf(buf,"|test-message-%d-abcdefghijklmnopqrstuvwxyz$",i+1);
                producer.PublishMesage(buf,routing_key);
                std::cout<<"Publish message:"<<buf<<std::endl;
                sleep(1);

		++i;
        }

        producer.CloseConnect();

        return 0;
}

