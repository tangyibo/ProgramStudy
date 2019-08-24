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

# 五、数据库表增量获取（计算）的方法
## 触发器
>在要抽取的表上建立需要的触发器，一般要建立插入、修改、删除三个触发器，每当源表中的数据发生变化，就被相应的触发器将变化的数据写入一个临时表，抽取线程从临时表中抽取数据，临时表中抽取过的数据被标记或删除。触发器方式的优点是数据抽取的性能较高，缺点是要求业务表建立触发器，对业务系统有一定的影响。 

## 时间戳
>它是一种基于快照比较的变化数据捕获方式，在源表上增加一个时间戳字段，系统中更新修改表数据的时候，同时修改时间戳字段的值。当进行数据抽取时，通过比较系统时间与时间戳字段的值来决定抽取哪些数据。有的数据库的时间戳支持自动更新，即表的其它字段的数据发生改变时，自动更新时间戳字段的值。有的数据库不支持时间戳的自动更新，这就要求业务系统在更新业务数据时，手工更新时间戳字段。同触发器方式一样，时间戳方式的性能也比较好，数据抽取相对清楚简单，但对业务系统也有很大的倾入性（加入额外的时间戳字段），特别是对不支持时间戳的自动更新的数据库，还要求业务系统进行额外的更新时间戳操作。另外，无法捕获对时间戳以前数据的delete和update操作,在数据准确性上受到了一定的限制。 

## 全表比对
>典型的全表比对的方式是采用MD5校验码。ETL工具事先为要抽取的表建立一个结构类似的MD5临时表，该临时表记录源表主键以及根据所有字段的数据计算出来的MD5校验码。每次进行数据抽取时，对源表和MD5临时表进行MD5校验码的比对，从而决定源表中的数据是新增、修改还是删除，同时更新MD5校验码。MD5方式的优点是对源系统的倾入性较小（仅需要建立一个MD5临时表），但缺点也是显而易见的，与触发器和时间戳方式中的主动通知不同，MD5方式是被动的进行全表数据的比对，性能较差。当表中没有主键或唯一列且含有重复记录时，MD5方式的准确性较差。 

## 日志分析
> 通过分析数据库自身的日志来判断变化的数据,例如MySQL数据库的binlog，可通过使用阿里开源的canal工具接收并将binlog推送至kafka中。

