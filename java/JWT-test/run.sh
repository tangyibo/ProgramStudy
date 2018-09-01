BINARY_JAR=bin/jwt-demo.jar
ENTRY_POINT=cn.com.main.JWTDemo
EXT_LIB_PATH=./lib
JVMFLAGS=-Dfile.encoding=UTF-8

for i in "$EXT_LIB_PATH"/*.jar
do
	CLASSPATH="$i:$CLASSPATH"
done

CLASSPATH="$BINARY_JAR:$CLASSPATH"

if [ "$JAVA_HOME" != "" ]; then
  JAVA="$JAVA_HOME/bin/java"
else
  JAVA=java
fi

COMMAND="$JAVA -cp $CLASSPATH $JVMFLAGS $ENTRY_POINT"
echo $COMMAND
$COMMAND
#java -Djava.ext.dirs=./lib -jar bin/jwt-demo.jar
