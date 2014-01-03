#!/bin/bash

JAVA_HOME_PATH=${JAVA_HOME}
JAVA_CMD="java"

javaCheck()
{
   $JAVA_CMD -version >& /dev/null && return 0
   echo "ERROR: Java VM ${JAVA_CMD} not found. Please set JAVA_HOME to the path of a valid Java VM installation"\
        "(the directory of the command \"java\" without the trailing \"bin\")."
   return 1
}

gcjCheck()
{
   $JAVA_CMD -version 2>&1 | grep -i gnu\ >& /dev/null || return 0;
   echo "ERROR: GNU Java installation found. JonDo will not be able to run with GNU Java."\
        "Please install a SUN JRE to run JonDo, (www.java.sun.com)."
   return 1
}

if [ "${JAVA_HOME}" ]; then
   if [ "${JAVA_HOME_PATH}" -a ! "${JAVA_HOME}" -ef "${JAVA_HOME_PATH}" ]; then
      echo "WARNING: JAVA_HOME is set to ${JAVA_HOME}, your specified path ${JAVA_HOME_PATH} will be ignored."\
           "If you want to use your specified path, please undefine JAVA_HOME."
   fi
   JAVA_HOME_PATH="${JAVA_HOME}"
fi

if [ "${JAVA_HOME_PATH}" ]; then
   JAVA_CMD="${JAVA_HOME_PATH}/bin/java"
fi

javaCheck && gcjCheck && $JAVA_CMD -Djava.net.preferIPv4Stack=true -XX:-UsePerfData -jar /usr/share/java/jondo/JAP.jar --hideUpdate --noSystemErrorLog $@


