#!/bin/sh

# Since version 4.1.1 onawards FFDec requires Java 8
export PATH="/usr/lib/jvm/java-8-openjdk/jre/bin:$PATH"
exec java -Djava.net.preferIPv4Stack=true -cp '/usr/share/java/ffdec/lib' -jar '/usr/share/java/ffdec/ffdec.jar' "$@"
