#!/bin/bash

PORT=$@

REQUEST=$'GET /tests/06-error/index.html HTTP/1.1\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
