#!/bin/bash
#
rm read expected &> /dev/null

PORT=$@

RANDOM_DATA=`dd if=/dev/urandom count=1 bs=1000 2> /dev/null | base64 | head -c 500`
echo -n "$RANDOM_DATA" > expected

EOL=$'\r'

REQUEST1="POST /write HTTP/1.1$EOL
Content-Length: 500$EOL
$EOL
$RANDOM_DATA"

REQUEST2=$'GET /read HTTP/1.1\r\n\r\n'

echo "$REQUEST1" | nc 127.0.0.1 $PORT &> /dev/null
wget http://127.0.0.1:$PORT/read &> /dev/null
diff --text read expected
