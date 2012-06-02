#!/bin/csh

set A = $<

if ( "x${A}" == "xfoo" ) echo "Foo (single line)"

if ( "x${A}" == "xfoo" ) then
    echo "Foo"
else if ( "x${A}" == "xbar" ) then
    echo "Bar"
else
    echo "Other"
endif

