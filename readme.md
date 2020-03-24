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

<details>
<summary>点此展开查看</summary>
  
1. 本项目定为大约每两个月发布一次正式版（同时 `develop` 分支代码合并进入 `master` 分支），版本号格式为 `X.X.0`（如`2.1.0`，`2.2.0`等），遇到重大问题需修复会及时提交新版本，欢迎大家随时提交Pull Request；
1. BUG修复和新特性一般会先发布成小版本作为临时测试版本（如`3.6.8.B`，即尾号不为0，并添加B，以区别于正式版），代码仅存在于 `develop` 分支中；
1. 目前最新版本号为 [![Maven Central](https://img.shields.io/maven-central/v/com.github.binarywang/wx-java.svg)](http://mvnrepository.com/artifact/com.github.binarywang/wx-java) ，也可以通过访问链接 [【微信支付】](http://search.maven.org/#search%7Cgav%7C1%7Cg%3A%22com.github.binarywang%22%20AND%20a%3A%22weixin-java-pay%22) 、[【微信小程序】](http://search.maven.org/#search%7Cgav%7C1%7Cg%3A%22com.github.binarywang%22%20AND%20a%3A%22weixin-java-miniapp%22) 、[【公众号】](http://search.maven.org/#search%7Cgav%7C1%7Cg%3A%22com.github.binarywang%22%20AND%20a%3A%22weixin-java-mp%22) 、[【企业微信】](http://search.maven.org/#search%7Cgav%7C1%7Cg%3A%22com.github.binarywang%22%20AND%20a%3A%22weixin-java-cp%22)、[【开放平台】](http://search.maven.org/#search%7Cgav%7C1%7Cg%3A%22com.github.binarywang%22%20AND%20a%3A%22weixin-java-open%22)
分别查看所有最新的版本。 

</details>
