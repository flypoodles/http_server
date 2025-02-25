#!/bin/bash

PORT=$@

REQUEST=$'GET /stats HTTP/1.1\r\n'
PING=$'GET /ping HTTP/1.1\r\n\r\n'

echo -n "$REQUEST" | nc 127.0.0.1 $PORT #0
echo ""
echo -n "$REQUEST" | nc 127.0.0.1 $PORT #1
echo ""
echo -n "$REQUEST" | nc 127.0.0.1 $PORT #2
echo ""
echo -n "$REQUEST" | nc 127.0.0.1 $PORT #3
echo ""
echo -n "$REQUEST" | nc 127.0.0.1 $PORT #4
echo ""
echo -n "$PING" | nc 127.0.0.1 $PORT &> /dev/null #5
echo -n "$REQUEST" | nc 127.0.0.1 $PORT #6
echo ""
curl -s http://127.0.0.1:$PORT/echo &> /dev/null #7
echo -n "$REQUEST" | nc 127.0.0.1 $PORT #8
echo ""
echo -n "$PING" | nc 127.0.0.1 $PORT &> /dev/null #5
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/echo &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/echo &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/read &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/echo &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/tests/05-files/index.html &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/Makefile &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/runtests.sh &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/tests/06-files/index.html &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/tests/06-error/index.html &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
curl -s http://127.0.0.1:$PORT/tests/06-error/index.html &> /dev/null
echo -n "$REQUEST" | nc 127.0.0.1 $PORT
echo ""
