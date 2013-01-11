#!/usr/bin/env bash
x11vnc -rfbauth ~/.vnc/passwd >& /dev/null &
sleep 1
./bin/main localhost 5900 junjijun

