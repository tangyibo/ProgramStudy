/*
 * CKafkaNode.cpp
 *
 *  Created: 2017年3月21日
 *   Author: tang
 */

#include "CKafkaNode.h"
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>

inline string StringFormat(const char* fmt, ...)
{
	int size = 256;
	std::string str;
	va_list ap;
	while (1)
	{
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *) str.c_str(), size, fmt, ap);
		va_end(ap);
		if (n > -1 && n < size)
		{
			str.resize(n);
			return str;
		}

		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}

///////////////////////////////////////////////////////////////////////////////////////
void CKafkaProducer::dr_msg_cb (rd_kafka_t *rk,const rd_kafka_message_t *rkmessage, void *opaque) 
{
        if (rkmessage->err)
	{
		char msg[10240] = {0};
		
     	snprintf(msg,10240, "%% Message delivery failed: %s\n",rd_kafka_err2str(rkmessage->err));
		 
	}
        else
	{
		char msg[1024]={0};
		
                snprintf(msg,1024,
                        "%% Message delivered (%zd bytes, "
                        "partition %PRId32\n",
                        rkmessage->len, rkmessage->partition);
		 
	}

        /* The rkmessage is destroyed automatically by librdkafka */
}

bool CKafkaProducer::Init(const string &broker_list, const string &topic_name)
{
	char errstr[512]={0};

	/* Kafka configuration */
	m_conf_handler = rd_kafka_conf_new();
	
	 /* Set the delivery report callback.
         * This callback will be called once per message to inform
         * the application if delivery succeeded or failed.
         * See dr_msg_cb() above. */
        rd_kafka_conf_set_dr_msg_cb(m_conf_handler, dr_msg_cb);

	/* Quick termination */
	char tmp[16]={0};
	snprintf(tmp, sizeof(tmp), "%i", SIGIO);
	rd_kafka_conf_res_t res=rd_kafka_conf_set(m_conf_handler, "internal.termination.signal", tmp, errstr, sizeof(errstr));
	if (res != RD_KAFKA_CONF_OK)
	{
		m_errstr=StringFormat("Call rd_kafka_conf_set() failed, error: %s", errstr);
		return false;
	}

	/*topic configuration*/
	m_topic_conf = rd_kafka_topic_conf_new();

	if (!(m_rk_handler = rd_kafka_new(RD_KAFKA_PRODUCER, m_conf_handler, errstr, sizeof(errstr))))
	{
		m_errstr=StringFormat("Failed to create new producer: %s", errstr);
		return false;
	}

	/* Add brokers */
	if (rd_kafka_brokers_add(m_rk_handler, broker_list.c_str()) == 0)
	{
		m_errstr=StringFormat("No valid brokers specified");
		return false;
	}

	/* Create topic */
	m_topic_rkt = rd_kafka_topic_new(m_rk_handler, topic_name.c_str(), m_topic_conf);

	/* Storage information*/
	m_topic_name=topic_name;
	m_broker_list=broker_list;

	return true;
}

my_kafka_result CKafkaProducer::Send(const string &data, int partition)
{
	if(data.empty())
		return MY_KAFKA_BUFFER_EMPTY;

	int ret = rd_kafka_produce(m_topic_rkt, partition, RD_KAFKA_MSG_F_COPY,
			(void*)data.c_str(), (size_t)data.size(), NULL, 0, NULL);
	if(ret == -1)
	{
		m_errstr=StringFormat("Failed to produce to topic[%s] partition[%i],error: %s",
				rd_kafka_topic_name(m_topic_rkt), partition,
				rd_kafka_err2str(rd_kafka_last_error()));

		rd_kafka_poll(m_rk_handler, 0);

		return MY_KAFKA_SEND_DATA_FAILED;
	}

	rd_kafka_poll(m_rk_handler, 0);

	return MY_KAFKA_OPERATOR_OK;
}

void CKafkaProducer::Close()
{
	if(!m_is_destroy)
	{
		/* Destroy topic */
		rd_kafka_topic_destroy (m_topic_rkt);

		/* Destroy the handle */
		rd_kafka_destroy (m_rk_handler);

		m_is_destroy=true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
bool CKafkaConsumer::Init(const string &broker_list, const string &topic_name,const string &group_name)
{
	char errstr[512];

	/* Kafka configuration */
	m_conf_handler = rd_kafka_conf_new();

	/*topic configuration*/
	m_topic_conf = rd_kafka_topic_conf_new();

	//设置group组,Client group id string. All clients sharing the same group.id belong to the same group.
	if (rd_kafka_conf_set(m_conf_handler, "group.id", group_name.c_str(),errstr, sizeof(errstr)) !=RD_KAFKA_CONF_OK)
	{
		m_errstr=StringFormat("Set [group.id] value failed,error: %s", errstr);
		return false;
	}

	//为了支持断点续传功能，需要将offset保存在一个地方，下次从这个offset开始
	//offset.store.method的可选值为none, file, broker
	//Offset commit store method: 'file' - local file store (offset.store.path,
	//et.al), 'broker' - broker commit store (requires Apache Kafka 0.8.2 or
	//later on the broker).
	if (rd_kafka_topic_conf_set(m_topic_conf, "offset.store.method",
			"broker",errstr, sizeof(errstr)) !=RD_KAFKA_CONF_OK)
	{
		m_errstr=StringFormat("Set [offset.store.method] value failed,error: %s", errstr);
		return false;
	}

	//Action to take when there is no initial offset in offset store or the desired
	//offset is out of range:
	//     @'smallest','earliest' - automatically reset the offset
	//                              to the smallest offset,
	//     @'largest','latest'    - automatically reset the offset to the largest offset,
	//     @'error' - trigger an error which is retrieved by consuming messages and
	//                   checking 'message->err'.
	string auto_offset_reset_value="latest";
	if(OFFSET_SMALLEST==m_read_offset)
	{
		auto_offset_reset_value="smallest";
	}
	else
	{
		auto_offset_reset_value="latest";
	}
	if (rd_kafka_topic_conf_set(m_topic_conf, "auto.offset.reset",
			auto_offset_reset_value.c_str(),errstr, sizeof(errstr)) !=RD_KAFKA_CONF_OK)
	{
		m_errstr=StringFormat("Set [auto.offset.reset] value failed,error: %s", errstr);
		return false;
	}

	rd_kafka_conf_set_default_topic_conf(m_conf_handler, m_topic_conf);
	if (!(m_rk_handler = rd_kafka_new(RD_KAFKA_CONSUMER, m_conf_handler,errstr, sizeof(errstr))))
	{
		m_errstr=StringFormat("Failed to create new consumer: %s",errstr);
		return false;
	}

	if (rd_kafka_brokers_add(m_rk_handler, broker_list.c_str()) == 0)
	{
		m_errstr=StringFormat("No valid brokers specified");
		return false;
	}

	rd_kafka_poll_set_consumer(m_rk_handler);

	m_topics_list = rd_kafka_topic_partition_list_new(m_partition_count);
	int32_t partition = -1;
	for(int i=0;i<m_partition_count;i++)
	{
		rd_kafka_topic_partition_list_add(m_topics_list, topic_name.c_str(), partition);
	}

	rd_kafka_resp_err_t err=RD_KAFKA_RESP_ERR_UNKNOWN;
	if ((err = rd_kafka_subscribe(m_rk_handler, m_topics_list)))
	{
		m_errstr=StringFormat("Failed to start consuming topics: %s",rd_kafka_err2str(err));
		return false;
	}

	/* Storage information*/
	m_topic_name=topic_name;
	m_broker_list=broker_list;

	return true;
}

my_kafka_result CKafkaConsumer::Recv(string &msg_data,int timeout_ms/*=1000*/)
{
	rd_kafka_message_t *rkmessage = rd_kafka_consumer_poll(m_rk_handler, timeout_ms);
	if (rkmessage)
	{
		if (rkmessage->err)
		{
			if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF)
			{
				m_errstr=StringFormat("Consumer reached end of %s [%d] message queue at offset %ld",
						rd_kafka_topic_name(rkmessage->rkt),
						rkmessage->partition,
						rkmessage->offset);
				return MY_KAFKA_RECV_DATA_END;
			}

			if (rkmessage->rkt)
			{
				m_errstr=StringFormat("Consume error for topic \"%s\" [%d] offset %ld: %s",
						rd_kafka_topic_name(rkmessage->rkt),
						rkmessage->partition,
						rkmessage->offset,
						rd_kafka_message_errstr(rkmessage));
			}
			else
			{
				m_errstr=StringFormat("Consumer error: %s: %s",
						rd_kafka_err2str(rkmessage->err),
						rd_kafka_message_errstr(rkmessage));
			}

			return MY_KAFKA_UNKNOWN_ERROR;
		}

		msg_data.assign((char*)rkmessage->payload);
		rd_kafka_message_destroy(rkmessage);

		return MY_KAFKA_OPERATOR_OK;
	}

	m_errstr="Call rd_kafka_consumer_poll() wait timeout";
	return MY_KAFKA_RECV_DATA_TIMEOUT;
}

void CKafkaConsumer::Close()
{
	rd_kafka_consumer_close(m_rk_handler);
	rd_kafka_topic_partition_list_destroy(m_topics_list);

	if(!m_is_destroy)
	{
		/* Destroy topic */
		rd_kafka_topic_destroy (m_topic_rkt);

		/* Destroy the handle */
		rd_kafka_destroy (m_rk_handler);

		m_is_destroy=true;
	}
}
