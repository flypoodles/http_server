#!/bin/bash

# Random 1024 header, should just fit

rm actual expected &> /dev/null

PORT=$@

RANDOM_DATA=`dd if=/dev/urandom count=1 bs=1000 2> /dev/null | base64 | head -c 995`
EOL=$'\r'
echo -n "HTTP/1.1 200 OK$EOL
Content-Length: 1004$EOL
$EOL
Header1: $RANDOM_DATA" > expected

REQUEST1="GET /echo HTTP/1.1$EOL
Header1: $RANDOM_DATA$EOL"

echo "$REQUEST1" | nc 127.0.0.1 $PORT 2> /dev/null > actual
diff actual expected
