#!/usr/bin/env bash

DS_PROG="DatabaseTest"
DS_MAIN="cn.com.ruijie.test.DbMySQL"

# JVMFLAGS JVM参数可以在这里设置
JVMFLAGS=-Dfile.encoding=UTF-8

DS_HOME="${BASH_SOURCE-$0}"
DS_HOME="$(dirname "${DS_HOME}")"
DS_HOME="$(cd "${DS_HOME}"; pwd)"
DS_HOME="$(cd "$(dirname ${DS_HOME})"; pwd)"

echo $DS_HOME

if [ "$JAVA_HOME" != "" ]; then
  JAVA="$JAVA_HOME/bin/java"
else
  JAVA=java
fi

mkdir -p $DS_HOME/logs
mkdir -p $DS_HOME/run

DS_CONSOLE_OUT=$DS_HOME/logs/System.out
DS_PID_FILE=$DS_HOME/run/$DS_PROG.pid

#把lib下的所有jar都加入到classpath中
for i in "$DS_HOME"/libs/*.jar
do
	CLASSPATH="$i:$CLASSPATH"
done

#echo $CLASSPATH

# 配置文件所在的路径
CONF_HOME="$DS_HOME/conf/"
#echo $CONF_HOME

DS_RUN_CMD="nohup $JAVA -cp $CLASSPATH $JVMFLAGS $DS_MAIN $CONF_HOME >> $DS_CONSOLE_OUT 2>&1 > /dev/null &"

case $1 in
start)
    echo "Starting $DS_PROG ... "
    if [ -f "$DS_PID_FILE" ]; then
      if kill -0 `cat "$DS_PID_FILE"` > /dev/null 2>&1; then
         echo $command already running as process `cat "$DS_PID_FILE"`. 
         exit 0
      fi
    fi

    echo $DS_RUN_CMD
    nohup "$JAVA" -cp "$CLASSPATH" $JVMFLAGS $DS_MAIN "$CONF_HOME" >> $DS_CONSOLE_OUT  2>&1 > /dev/null &

    if [ $? -eq 0 ]
    then
      if /bin/echo -n $! > "$DS_PID_FILE"
      then
        sleep 1
        echo "STARTED"
      else
        echo "FAILED TO WRITE PID"
        exit 1
      fi
    else
      echo "$DS_PROG NOT START"
      exit 1
    fi
;;
restart)
    sh $0 stop
    sleep 3
    sh $0 start
;;
stop)
    echo "Stopping $DS_PROG ... "
    if [ ! -f "$DS_PID_FILE" ]
    then
      echo "no $DS_PROG to started (could not find file $DS_PID_FILE)"
    else
      kill -9 $(cat "$DS_PID_FILE")
      rm "$DS_PID_FILE"
      echo "STOPPED"
    fi
    exit 0
;;
*)
    echo "Usage: $0 {start|stop|restart}" >&2
esac
