#!/bin/bash

PORT=$@

# Generates a random number of random headers and echoes them

for i in $(seq 20); do
    EOL=$'\r\n'
    REQUEST="GET /echo HTTP/1.1"
    EXPECTED=""

    VARS=$(($RANDOM % 20))

    NAME_LEN=$((1 + $RANDOM % 20))
    NAME=`dd if=/dev/urandom count=1 bs=20 2> /dev/null | base64 | head -c $NAME_LEN | sed s/://g`
    CONTENT_LEN=$((1 + $RANDOM % 20))
    CONTENT=`dd if=/dev/urandom count=1 bs=20 2> /dev/null | base64 | head -c $CONTENT_LEN | sed s/://g`

    VAR="$NAME: $CONTENT"
    REQUEST="$REQUEST$EOL$VAR"
    EXPECTED="$VAR"

    for i in $(seq $VARS); do
        NAME_LEN=$((1 + $RANDOM % 20))
        NAME=`dd if=/dev/urandom count=1 bs=20 2> /dev/null | base64 | head -c $NAME_LEN | sed s/://g`
        CONTENT_LEN=$((1 + $RANDOM % 20))
        CONTENT=`dd if=/dev/urandom count=1 bs=20 2> /dev/null | base64 | head -c $CONTENT_LEN | sed s/://g`

        VAR="$NAME: $CONTENT"
        REQUEST="$REQUEST$EOL$VAR"
        EXPECTED="$EXPECTED$EOL$VAR"
    done

    REQUEST="$REQUEST$EOL$EOL"

    echo -n "$REQUEST" | nc 127.0.0.1 $PORT | tail -n +4 > actual
    echo -n "$EXPECTED" > expected
    diff actual expected
done
