#!/bin/bash

PORT=$@

REQUEST1=$'POST /write HTTP/1.1\r
Header1: Value1\r
Content-Length: 24\r
Header2: Value2\r
Header3: Value3\r
Header4: Value4\r
Header5: Value5\r
Header6: Value6\r
\r
This is the data to save'

REQUEST2=$'GET /read HTTP/1.1\r\n\r\n'

echo -n "$REQUEST1" | nc 127.0.0.1 $PORT
echo ""
echo -n "$REQUEST2" | nc 127.0.0.1 $PORT
