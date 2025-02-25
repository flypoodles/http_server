#!/bin/bash

PORT=$@

curl -s http://127.0.0.1:$PORT/echo | sed s/$PORT/XXXXX/g | sed s/curl\\/.*$/curl\\/XXX/g
