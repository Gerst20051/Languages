#!/bin/csh

set FOO = "x"
while ("x${FOO}" != "x")
    set FOO = `cat`
end

