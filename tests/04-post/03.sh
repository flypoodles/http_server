#!/bin/bash

PORT=$@

REQUEST1=$'POST /write HTTP/1.1\r
Content-Length: 98\r
\r
Write a lot of data on the first time to make sure the second write is not combined with the first'

REQUEST2=$'POST /write HTTP/1.1\r
Content-Length: 17\r
\r
Write little data'

REQUEST3=$'GET /read HTTP/1.1\r\n\r\n'


echo "$REQUEST1" | nc 127.0.0.1 $PORT
echo ""
echo "$REQUEST2" | nc 127.0.0.1 $PORT
echo ""
echo "$REQUEST3" | nc 127.0.0.1 $PORT
echo ""
