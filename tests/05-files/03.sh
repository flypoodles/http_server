#!/bin/bash
#
rm read expected &> /dev/null

PORT=$@

RANDOM_DATA=`dd if=/dev/urandom count=5 bs=1024 2> /dev/null | base64`
echo -n "$RANDOM_DATA" > expected
echo -n "$RANDOM_DATA" > file

curl -s http://127.0.0.1:$PORT/file &> actual
diff expected actual
