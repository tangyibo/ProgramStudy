# 设置你G要生成的jar包的文件名
# Set the file name of your jar package:
JAR_PKG =TestMongo.jar

# 设置你的项目的入口点
# Set your entry point of your java app:
ENTRY_POINT = com.learn.test.TestMongo

# 设置你的java编译器
# Set your java compiler here:
JAVAC = javac

# 设置你的编译选项
# Set compile options here:
JFLAGS = -encoding UTF-8 -g

# 建议的用法如下：
# 如果你定义的类包含在某个包里：请自己在src下建立相应的目录层次。
# 最终的目录结构如下：
# ├── bin
# │     └── a.jar
# │     └── test
# │             ├── A.class
# │             └── B.class
# ├── lib
# │      └── X.jar
# │      └── Y.jar
# ├── makefile
# ├── conf
# │     └── config.properties
# │     └── log4j.properties
# └── src
#        └── test
#                ├── A.java
#                └── B.java

# make build: 编译，在bin目录下生成 java classes。
# make clean: 清理编译结果，以便重新编译
# make rebuild: 清理编译结果，重新编译。
# make run: make 之后，可以通过make run查看运行结果。
# make jar: 生成可执行的jar包。

#############下面的内容建议不要修改####################
SOURCE_FILES:=$(shell find src -name *.java)
SRC_SBU_DIR=$(shell find src -type d)
VAR_TMP_STR=$(shell echo $(SRC_SBU_DIR) | sed 's/[ ][ ]*/:/g')
MYCLASSPATH=$(shell echo $(VAR_TMP_STR) | sed 's/src/bin/g')

# show help message by default
Default:
	@echo "make build: build project."
	@echo "make clean: clear classes generated."
	@echo "make rebuild: rebuild project."
	@echo "make run: run your app."
	@echo "make jar: package your project into a executable jar."

build:  $(SOURCE_FILES)
	@echo 'Building project files'
	@echo 'Invoking:JAVAC Compiler'
	$(JAVAC) -classpath $(MYCLASSPATH)  -Djava.ext.dirs=lib -d bin $(JFLAGS) $(SOURCE_FILES)
	@echo 'Finished building files'
	@echo ' '

rebuild: clean build

.PHONY: clean run jar

clean:
	-rm -frv bin/*
	-rm -f bin/$(JAR_PKG)

run:
	java -classpath $(MYCLASSPATH) -Djava.ext.dirs=lib  $(ENTRY_POINT)

jar:
	jar cvfe bin/$(JAR_PKG) $(ENTRY_POINT) -C bin .
