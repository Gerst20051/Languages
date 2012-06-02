#!/bin/sh
echo # Start on a new line
printf "\e[19D" # move right 19 columns to column 20
printf "\e[3g" # clear all tab stops
printf "\e[W" # set a new tab stop
printf "\e[19C" # move back to the left
printf "Tab test\tThis starts at column 20."
printf "\n"
reset

