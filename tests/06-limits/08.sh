#!/bin/bash

# Write 1024 bytes with random data, should just fit

RANDOM_DATA=`dd if=/dev/urandom count=1 bs=1000 2> /dev/null | base64 | head -c 1024`

rm actual expected &> /dev/null

EOL=$'\r'

echo -n "HTTP/1.1 200 OK$EOL
Content-Length: 1024$EOL
$EOL
$RANDOM_DATA
HTTP/1.1 200 OK$EOL
Content-Length: 1024$EOL
$EOL
$RANDOM_DATA" > expected

PORT=$@

REQUEST1="POST /write HTTP/1.1$EOL
Content-Length: 1024$EOL
$EOL
$RANDOM_DATA"

REQUEST2=$'GET /read HTTP/1.1\r\n\r\n'

for run in {1..10}; do
    echo "$REQUEST1" | nc 127.0.0.1 $PORT 2> /dev/null > actual
    echo "" >> actual
    echo "$REQUEST2" | nc 127.0.0.1 $PORT 2> /dev/null >> actual
    diff actual expected
done
