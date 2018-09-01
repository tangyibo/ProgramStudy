#ifndef _RABBIT_MQ_CPP_H_
#define _RABBIT_MQ_CPP_H_
#include "amqp.h"
#include "amqp_framing.h"
#include <string>
using std::string;

class RabbitmqNode
{

protected:
        RabbitmqNode(const int channel=1);
        ~RabbitmqNode();
public:
        const string GetLastError() const { return m_errstr;}

        bool OpenConnect(const string &host,const int &port,const bool use_ssl=false);
        bool Login(const string &user,const string &passwd,const string &virtualhost="/");
        bool CloseConnect();
protected:
        string amqp_error(amqp_rpc_reply_t x, char const *context);

protected:
        //error message
        string m_errstr;

        bool m_isConnected;
        bool m_isAuthLogin;
  
        //connection
        amqp_connection_state_t m_conn;

        //channel
        int m_channelId;
        
        //connect information
        string  m_strHost;
        int     m_nPort;
        string  m_strUserName;
        string  m_strPassword;
        string  m_strVirtualHost;
};

class RabbitProducer: public RabbitmqNode
{
public:
        RabbitProducer();
        ~RabbitProducer();

        bool Init(const string &queue,const string &exchange);
        bool PublishMesage(const string &msg,const string &routing_key);
private:
        string m_strQueue;
        string m_strExchange;
};


class RabbitConsumer: public RabbitmqNode
{
public:
        RabbitConsumer();
        ~RabbitConsumer();

        bool Init(const string &queue,const string &exchange,const string &routing_key,const string &consumer_tag);
        int RecvMessage(string &data,const int &microseconds=0);

private:
        string m_strQueue;
        string m_strExchange; 
};

#endif
