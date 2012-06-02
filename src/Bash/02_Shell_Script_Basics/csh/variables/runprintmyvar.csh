#!/bin/csh

set MYVAR = 3
env MYVAR=7 ./printmyvar.csh
echo "MYVAR IS $MYVAR"

