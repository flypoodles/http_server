#!/bin/bash

PORT=$@

curl -s http://127.0.0.1:$PORT/tests/06-error
curl -s http://127.0.0.1:$PORT/tests/06-error
curl -s http://127.0.0.1:$PORT/tests/06-error
echo -n ""
