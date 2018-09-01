#include <iostream>
#include "RabbitmqCpp.h"
#include <unistd.h>

int main(int arg,char *argv[])
{
        string host="127.0.0.1";
        int port=5672;
        string user="test";
        string passwd="123456";
        string exchange="ex_name";
        string queue="queue_name";
        string routing_key="routing_key";
        string consumer_tag="consumer_tag";
        string vhost="private";

	RabbitConsumer consumer;
        if(!consumer.OpenConnect(host,port))
        {   
                std::cout<<consumer.GetLastError()<<std::endl;
                return 1;
        }   

        if(!consumer.Login(user,passwd,vhost))
        {   
                std::cout<<consumer.GetLastError()<<std::endl;
                return 1;
        }   

        if(!consumer.Init(queue,exchange,routing_key,consumer_tag))
        {   
                std::cout<<consumer.GetLastError()<<std::endl;
                return 1;
        }   

        string data;
        while(1)
        {
                int ret= consumer.RecvMessage(data);
                if(0==ret)
                {
                        std::cout<<"Recv:"<<data<<std::endl;
                }
                else
                {
                        std::cout<<consumer.GetLastError()<<std::endl;
                }
        }

        consumer.CloseConnect();

        return 0;
}

