# Program-Study 基础知识温习

# 一、内容

(1) reflect 反射

(2) algorithm 算法

(3) RTTI（Run-Time Type Identification，运行时类型识别）与 强制类型转换dynamic_cast

(4) boost库学习

(5) 协程

# 二、编程语言 

 (1) c++

 (2) python
 
 
# 三、编码规范
（1）google的C++编码规范
  http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/


下载POM文件中的依赖：
> mvn -f pom.xml dependency:copy-dependencies

# 四、kafka常用命令
export KAFKA_OPTS="-Djava.security.auth.login.config=/opt/kafka/config/kafka_server_jaas.conf"

## (1) 创建 topic
bin/kafka-topics.sh --zookeeper localhost:2181/kafka --create --topic real_data_sync --partitions 1 --replication-factor 1

## (2) 列举 topic
bin/kafka-topics.sh -zookeeper localhost:2181/kafka --list

## (3) 详细信息 topic
bin/kafka-topics.sh -zookeeper localhost:2181/kafka --describe --topic real_data_sync

## (4) 删除 topic
bin/kafka-topics.sh --delete --zookeeper localhost:2181/kafka --topic test

## (5) 消费数据 topic
bin/kafka-console-consumer.sh --bootstrap-server localhost:19099 --topic test --group aaaa   --from-beginning  --consumer-property security.protocol=SASL_PLAINTEXT  --consumer-property sasl.mechanism=PLAIN

## (6) 生产数据 topic
bin/kafka-console-producer.sh --broker-list localhost:19099 --topic test --producer-property security.protocol=SASL_PLAINTEXT --producer-property sasl.mechanism=PLAIN

## (7) 修改 topic 分区
bin/kafka-topics.sh --alter --zookeeper localhost:2181/kafka --topic data_convert --partitions 2
