         <<相关说明>>
================================
编写作者 ：TangYibo
邮箱地址 : inrgihc@126.com
================================

一、简介
  本程序为学习kafka的Java环境开发而编写的示例程序,包括生产者与消费者两种模式。

二、编译
（1）确保已经安装和配置了JDK环境
（2）这里采用makefile编译,具体详见makefile
 $ make clean   清理上次编译生成的文件
 $ make build   编译
 $ make run     运行

三、说课
 1、配置文件config.properties中的kafka-brokerlist的IP地址需要在/etc/hosts做好映射



