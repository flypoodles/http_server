#!/bin/bash

# Random header over 1024, should not crash server and trimmed at 1024

rm actual expected &> /dev/null

PORT=$@

RANDOM_DATA=`dd if=/dev/urandom count=1 bs=1000 2> /dev/null | base64 | head -c 995`
RANDOM_DATA_OVER=`dd if=/dev/urandom count=2 bs=1000 2> /dev/null | base64 | head -c 500`

EOL=$'\r'

echo -n "HTTP/1.1 200 OK$EOL
Content-Length: 1004$EOL
$EOL
Header1: $RANDOM_DATA" > expected

REQUEST1="GET /echo HTTP/1.1$EOL
Header1: $RANDOM_DATA$RANDOM_DATA_OVER"

echo "$REQUEST1" | nc 127.0.0.1 $PORT 2> /dev/null > actual
diff actual expected
