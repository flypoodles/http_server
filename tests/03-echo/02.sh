#!/bin/bash

PORT=$@

REQUEST=$'GET /echo HTTP/1.1\r
Header: Value\r\n\r\n'

echo "$REQUEST" | nc 127.0.0.1 $PORT
