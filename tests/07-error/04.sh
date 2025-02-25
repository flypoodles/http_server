#!/bin/bash

PORT=$@

REQUEST=$'GET /write HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
echo ""

REQUEST=$'POST /ping HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
echo ""

REQUEST=$'POST /read HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
echo ""

REQUEST=$'POST /tests/05-files/index.html HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
echo ""

REQUEST=$'GET/tests/05-files/index.html HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
echo ""

REQUEST=$'GETGETGET /tests/05-files/index.html HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
