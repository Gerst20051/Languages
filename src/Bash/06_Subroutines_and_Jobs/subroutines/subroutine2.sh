#!/bin/sh

mysub()
{
        return 3
}

mysub "This is an arg"
echo "Subroutine returned $?"
