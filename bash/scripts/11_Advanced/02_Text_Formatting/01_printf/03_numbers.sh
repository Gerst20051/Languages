#!/bin/sh

GPA="3.885"

printf "%f | whatever\n" "$GPA"
printf "%20f | whatever\n" "$GPA"
printf "%+20f | whatever\n" "$GPA"
printf "%+020f | whatever\n" "$GPA"
printf "%-20f | whatever\n" "$GPA"
printf "%- 20f | whatever\n" "$GPA"


