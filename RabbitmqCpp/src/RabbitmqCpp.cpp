#include "RabbitmqCpp.h"
#include "amqp_tcp_socket.h"  
#include "amqp.h"  
#include "amqp_framing.h" 
#include "amqp_ssl_socket.h"
#include <stdio.h>
#include <cstring>
#include <sys/time.h>

RabbitmqNode::RabbitmqNode(const int channel/*=1*/)
:m_errstr("")
,m_isConnected(false)
,m_isAuthLogin(false)
,m_channelId(channel)
{
}

RabbitmqNode::~RabbitmqNode()
{
        if(m_isConnected)
        {
                CloseConnect();

                m_isConnected=false;
                m_isAuthLogin=false;
        }
}

string RabbitmqNode::amqp_error(amqp_rpc_reply_t x, char const *context)
{
        char buffer[1024];
        switch (x.reply_type) 
        {
                case AMQP_RESPONSE_NORMAL:
                        return "";

                case AMQP_RESPONSE_NONE:
                        sprintf(buffer, "%s: missing RPC reply type!", context);
                        return string(buffer);

                case AMQP_RESPONSE_LIBRARY_EXCEPTION:
                        sprintf(buffer, "%s: %s", context, amqp_error_string2(x.library_error));
                        return string(buffer);

                case AMQP_RESPONSE_SERVER_EXCEPTION:
                        switch (x.reply.id) 
                        {
                                case AMQP_CONNECTION_CLOSE_METHOD: 
                                {
                                       amqp_connection_close_t *m =(amqp_connection_close_t *)x.reply.decoded;
                                        sprintf(buffer,"%s: server connection error %uh, message: %.*s",
                                                      context, m->reply_code, (int)m->reply_text.len,
                                                      (char *)m->reply_text.bytes);
                                        return string(buffer);
                                }
                                case AMQP_CHANNEL_CLOSE_METHOD: 
                                {
                                       amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
                                       sprintf(buffer,"%s: server channel error %uh, message: %.*s",
                                                context, m->reply_code, (int)m->reply_text.len,
                                                (char *)m->reply_text.bytes);
                                       return string(buffer);
                                }
                                default:
                                       sprintf(buffer,"%s: unknown server error, method id 0x%08X",context, x.reply.id);
                                       return string(buffer);
                        }
                        break;
        }

        return "";
}

bool RabbitmqNode::OpenConnect(const string &host,const int &port,const bool use_ssl/*=false*/)
{
        m_strHost=host;
        m_nPort=port;

        m_conn= amqp_new_connection();
        
        amqp_socket_t *socket;
        if(use_ssl)
        {
                socket=amqp_ssl_socket_new(m_conn);
        }
        else
        {
                socket = amqp_tcp_socket_new(m_conn);
        }

        if (!socket) 
        {
                if(use_ssl) m_errstr="Failed in creating SSL/TLS socket!";
                else m_errstr="creating TCP socket";

                return false;
        }

        if(use_ssl)
        {
                amqp_ssl_socket_set_verify_peer(socket, 0);
                amqp_ssl_socket_set_verify_hostname(socket, 0);
        }

        int status = amqp_socket_open(socket,m_strHost.c_str(), m_nPort);
        if(status)
        {
                if(use_ssl) m_errstr="Failed in opening SSL/TLS connection!";
                else m_errstr="opening TCP socket";

                return false;
        }

        m_isConnected=true;
        return true;
}

bool RabbitmqNode::Login(const string &user,const string &passwd,const string &vhost)
{
        if(!m_isConnected)
        {
                m_errstr="Please openconnect() first!";
                return false;
        }

        m_strUserName=user;
        m_strPassword=passwd;
        m_strVirtualHost=vhost;

        amqp_rpc_reply_t ret=amqp_login(m_conn,
                        m_strVirtualHost.c_str(), 
                        0, 
                        131072, 
                        0, 
                        AMQP_SASL_METHOD_PLAIN,
                        m_strUserName.c_str(),
                        m_strPassword.c_str()
                        );
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Logging in");
                return false;
        }

        amqp_channel_open(m_conn,m_channelId);
        ret=amqp_get_rpc_reply(m_conn);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Opening channel");
                return false;
        }

        m_isAuthLogin=true;
        
        return true;
}

bool RabbitmqNode::CloseConnect()
{
        if(!m_isConnected)
        {
                return true;
        }

        amqp_rpc_reply_t  ret;

        ret=amqp_channel_close(m_conn,m_channelId, AMQP_REPLY_SUCCESS);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Closing channel");
                return false;
        }

        ret=amqp_connection_close(m_conn, AMQP_REPLY_SUCCESS);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Closing connection");
                return false;
        }

        int r=amqp_destroy_connection(m_conn);
        if(r<0)
        {
                char buf[128];
                sprintf(buf,"%s: %s","Ending connection", amqp_error_string2(r));
                m_errstr=string(buf);
                return false;
        }

        return true;
}

////////////////////////////////////////////////////////////
RabbitProducer::RabbitProducer()
{
}

RabbitProducer::~RabbitProducer()
{
}

bool RabbitProducer::Init(const string &queue,const string &exchange)
{
        m_strQueue=queue;
        m_strExchange=exchange;

	amqp_queue_bind(m_conn,
			m_channelId,
			amqp_cstring_bytes(m_strQueue.c_str()),//binding key
			amqp_cstring_bytes(m_strExchange.c_str()),
			amqp_cstring_bytes(m_strQueue.c_str()),//binding key
			amqp_empty_table
		       );
        amqp_rpc_reply_t ret=amqp_get_rpc_reply(m_conn);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Binding");
                return false;
        }

	return true;
}

bool RabbitProducer::PublishMesage(const string &msg,const string &routing_key)
{
        if(!m_isConnected || !m_isAuthLogin)
        {
                m_errstr="Please OpenConnect() and Login() first!";
                return false;
        }

        amqp_basic_properties_t props;
        props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
        props.content_type = amqp_cstring_bytes("text/plain");
        props.delivery_mode = 2; /* persistent delivery mode */

        int ret=amqp_basic_publish(m_conn,
                        m_channelId,
                        amqp_cstring_bytes(m_strExchange.c_str()),
                        amqp_cstring_bytes(routing_key.c_str()),//routing key
                        0,
                        0,
                        &props,
                        amqp_cstring_bytes(msg.c_str())
                        );

        if(ret<0)
        {
                char buf[128];
                sprintf(buf,"%s: %s","Publish message", amqp_error_string2(ret));
                m_errstr=string(buf);
                return false;
        }

        return true;
}

////////////////////////////////////////////////////////////////////////////

RabbitConsumer::RabbitConsumer()
{
}

RabbitConsumer::~RabbitConsumer()
{
}

bool RabbitConsumer::Init(const string &queue,const string &exchange,const string &routing_key,const string &consumer_tag)
{
        m_strQueue=queue;
        m_strExchange=exchange;

	amqp_queue_declare_ok_t *r=amqp_queue_declare(m_conn,
			m_channelId, 
			amqp_empty_bytes, 
			0, 
			0, 
			0, 
			1, 
			amqp_empty_table
			);
        amqp_rpc_reply_t ret=amqp_get_rpc_reply(m_conn);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Declaring queue");
                return false;
        }

        amqp_bytes_t queuename=amqp_bytes_malloc_dup(r->queue);
        if(queuename.bytes ==NULL)
        {
                m_errstr="Out of memory while copying queue name";
                return false;
        }

        amqp_queue_bind(m_conn,
                        m_channelId, 
                        queuename, 
                        amqp_cstring_bytes(m_strExchange.c_str()),//exchage
                        amqp_cstring_bytes(routing_key.c_str()),//routing key 
                        amqp_empty_table
                       );
        ret=amqp_get_rpc_reply(m_conn);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Binding queue");
                return false;
        }

        amqp_basic_consume(m_conn,
                        m_channelId, 
                        queuename, 
                        amqp_cstring_bytes(consumer_tag.c_str()),
                        0,
                        1,
                        0,
                        amqp_empty_table
                        );
        ret=amqp_get_rpc_reply(m_conn);
        if(AMQP_RESPONSE_NORMAL!=ret.reply_type)
        {
                m_errstr=amqp_error(ret,"Consume message");
                return false;
        }
        return true;
}

int RabbitConsumer::RecvMessage(string &data,const int &microseconds/*=0*/)
{
        amqp_envelope_t envelope;
        amqp_maybe_release_buffers(m_conn);

        amqp_rpc_reply_t ret;

        if(microseconds>0)
        {
                struct timeval tv;
                gettimeofday(&tv, NULL);
                tv.tv_usec+=microseconds;

                ret = amqp_consume_message(m_conn, &envelope, &tv, 0);
        }
        else
        {
 	        ret = amqp_consume_message(m_conn, &envelope, NULL, 0);
        }
        if (AMQP_RESPONSE_NORMAL != ret.reply_type)
        {
                char buf[16];
                sprintf(buf,"%d",ret.reply_type);
                m_errstr="Error where consume message,return code:"+string(buf);
                return -1;
        }

#ifdef __LIBRABBIT_MQ_DEBUG__
	printf("Delivery %u, exchange %.*s routingkey %.*s\n",  
			(unsigned) envelope.delivery_tag,  
			(int) envelope.exchange.len, (char *) envelope.exchange.bytes,  
			(int) envelope.routing_key.len, (char *) envelope.routing_key.bytes);  

	if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {  
		printf("Content-type: %.*s\n",  
				(int) envelope.message.properties.content_type.len,  
				(char *) envelope.message.properties.content_type.bytes);  
	}
	printf("Content %.*s",(int)envelope.message.body.len,(char *)envelope.message.body.bytes);  
	printf("----\n");  
#else
        data.assign((char*)envelope.message.body.bytes,envelope.message.body.len);
#endif	

        amqp_destroy_envelope(&envelope);

        return 0;
}
