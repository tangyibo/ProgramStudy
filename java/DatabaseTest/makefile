# 设置你G要生成的jar包的文件名
# Set the file name of your jar package:
JAR_PKG =dbtest.jar

# 设置你的编译文件所在到目录(不要为build)
# Set directory for build files(not equal 'build')
BUILD_DIR=dist

# 设置你的项目的入口点
# Set your entry point of your java app:
ENTRY_POINT = cn.com.ruijie.test.DbMySQL

# 设置你的java编译器
# Set your java compiler here:
JAVAC = javac

# 设置你的编译选项
# Set compile options here:
JFLAGS = -encoding UTF-8 -g

# 建议的用法如下：
# 如果你定义的类包含在某个包里：请自己在src下建立相应的目录层次。
# 最终的目录结构如下：
# ├── build
# │     └── test
# │             ├── A.class
# │             └── B.class
# ├── libs
# │      └── X.jar
# │      └── Y.jar
# ├── makefile
# ├── conf
# │     └── config.properties
# │     └── log4j.properties
# ├── bin
# │     └── startup.sh
# └── src
#        └── test
#                ├── A.java
#                └── B.java

# make build: 编译，在$(BUILD_DIR)目录下生成 java classes。
# make clean: 清理编译结果，以便重新编译
# make rebuild: 清理编译结果，重新编译。
# make run: make 之后，可以通过make run查看运行结果。
# make jar: 生成可执行的jar包。

#############下面的内容建议不要修改####################
SOURCE_FILES:=$(shell find src -name *.java)
SRC_SBU_DIR=$(shell find src -type d)
VAR_TMP_STR=$(shell echo $(SRC_SBU_DIR) | sed 's/[ ][ ]*/:/g')
MYCLASSPATH=$(shell echo $(VAR_TMP_STR) | sed 's/src/$(BIN_DIR)/g')

build:  $(SOURCE_FILES)
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR) ; fi;
	@echo 'Building project files'
	@echo 'Invoking:JAVAC Compiler'
	$(JAVAC) -classpath $(MYCLASSPATH)  -Djava.ext.dirs=libs -d $(BUILD_DIR) $(JFLAGS) $(SOURCE_FILES)
	@echo 'Finished building files'
	@echo ' '

# show help message by default
help:
	@echo "make all: build project classes and jar."
	@echo "make build: only build project classes."
	@echo "make clean: clear classes generated and jar file."
	@echo "make rebuild: rebuild project classes."
	@echo "make run: run your app."
	@echo "make jar: package your project into a executable jar."

all: build jar

rebuild: clean all

.PHONY: all clean run jar

clean:
	-rm -frv $(BUILD_DIR)/*
	-rm -f libs/$(JAR_PKG)
	-rm -rf run logs
run:
	java -classpath $(MYCLASSPATH) -Djava.ext.dirs=libs  $(ENTRY_POINT) ./conf

jar:
	jar cvfe libs/$(JAR_PKG) $(ENTRY_POINT) -C $(BUILD_DIR) .
