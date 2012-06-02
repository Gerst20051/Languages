#!/bin/sh
GLOBAL_PATH="$PATH"
PATH=/usr/local/bin

/usr/bin/env PATH="$GLOBAL_PATH" /bin/ls
