# kettle java api 使用配置文档
## 1、 文档地址
- https://javadoc.pentaho.com/
 
## 2、源码地址
- https://github.com/pentaho/metastore/releases/tag/8.3.0.3-RC
- https://github.com/pentaho/pentaho-kettle/releases/tag/8.3.0.3-RC

## 3、pom.xml配置
###(1) 依赖
```
		<dependency>
			<groupId>pentaho-kettle</groupId>
			<artifactId>kettle-core</artifactId>
			<version>6.1.0.1-196</version>
		</dependency>
		<dependency>
			<groupId>pentaho-kettle</groupId>
			<artifactId>kettle-engine</artifactId>
			<version>6.1.0.1-196</version>
		</dependency>
		<dependency>
			<groupId>pentaho</groupId>
			<artifactId>metastore</artifactId>
			<version>6.1.0.1-196</version>
		</dependency>
```
### (2) 仓库
```
	<repositories>
		<repository>
			<id>maven-ali</id>
			<url>http://maven.aliyun.com/nexus/content/groups/public//</url>
			<releases>
				<enabled>true</enabled>
			</releases>
			<snapshots>
				<enabled>true</enabled>
				<updatePolicy>always</updatePolicy>
				<checksumPolicy>fail</checksumPolicy>
			</snapshots>
		</repository>
		<repository>
			<id>pentaho-public</id>
			<name>Pentaho Public</name>
			<url>http://nexus.pentaho.org/content/groups/omni/</url>
			<releases>
				<enabled>true</enabled>
				<updatePolicy>daily</updatePolicy>
			</releases>
			<snapshots>
				<enabled>true</enabled>
				<updatePolicy>interval:15</updatePolicy>
			</snapshots>
		</repository>
	</repositories>
```


