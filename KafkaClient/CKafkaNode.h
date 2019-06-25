/*
 * CKafkaNode.h
 *
 *  Created: 2017年3月21日
 *   Author: tang
 */

#ifndef SRC_CKAFKANODE_H_
#define SRC_CKAFKANODE_H_
#include "librdkafka/rdkafka.h"
#include <string>

using std::string;

typedef enum
{
	MY_KAFKA_OPERATOR_OK = 0,   /**< Operation okay */
	MY_KAFKA_UNKNOWN_ERROR = -1,/**< Unknown Error. */
	MY_KAFKA_BUFFER_EMPTY = -2, /**< Buffer Empty. */
	MY_KAFKA_SEND_DATA_FAILED=-3,/**< Producer Put Data failed. */
	MY_KAFKA_RECV_DATA_END=-4,   /**< Consumer Get End of Data. */
	MY_KAFKA_RECV_DATA_TIMEOUT=-5,/**< Consumer Get Data Timeout. */
} my_kafka_result;

class CKafkaNode
{
public:
	CKafkaNode():m_conf_handler(0),m_rk_handler(0),m_topic_conf(0),m_topic_rkt(0)
		,m_errstr(""),m_topic_name(""),m_broker_list(""){}
	virtual ~CKafkaNode(){}

	const string GetLastError(){return m_errstr;}
	const string TopicName(){ return m_topic_name;}
	const string BrokerList(){ return m_broker_list;}

protected:
	//rd handler
	rd_kafka_conf_t *m_conf_handler;
	rd_kafka_t *m_rk_handler;

	//topic
	rd_kafka_topic_conf_t *m_topic_conf;
	rd_kafka_topic_t *m_topic_rkt;

	//error message
	string m_errstr;

public:
	string m_topic_name;
	string m_broker_list;
};

/*
 * Kafka Producer
 */
class CKafkaProducer:public CKafkaNode
{
public:
	CKafkaProducer():m_is_destroy(false),m_topic_name(""),m_broker_list(""){}
	~CKafkaProducer(){}

public:
	bool Init(const string &broker_list, const string &topic_name);
	my_kafka_result Send(const string &data,int partition=RD_KAFKA_PARTITION_UA);
	void Close();

private:
	bool m_is_destroy;
        static void  dr_msg_cb (rd_kafka_t *rk,const rd_kafka_message_t *rkmessage, void *opaque);
public:
	string m_topic_name;
	string m_broker_list;
};

/*
 * Kafka Consumer
 */
class CKafkaConsumer : public CKafkaNode
{
public:
	typedef enum{
		OFFSET_SMALLEST=0,
		OFFSET_LARGEST=1,
		OFFSET_ARYWHERE=2,
	}read_offset;

	CKafkaConsumer(const int partition_count=10,const read_offset offset=OFFSET_LARGEST)
	:m_is_destroy(false),m_topics_list(0),m_group_name(""),m_partition_count(partition_count)
	,m_read_offset(offset){}
	~CKafkaConsumer(){}

	bool Init(const string &broker_list, const string &topic_name,const string &group_name);
	my_kafka_result Recv(string &msg_data,int timeout_ms=1000);
	void Close();

private:
	bool m_is_destroy;
	rd_kafka_topic_partition_list_t *m_topics_list;

public:
	string m_group_name;
	int m_partition_count;
	read_offset m_read_offset;
};

#endif /* SRC_CKAFKANODE_H_ */
