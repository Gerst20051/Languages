@echo off
::  Current delay is 60 seconds. The "1" at the end of CHOICE
::  gives a one-second delay. LOOP10 and LOOP6 work together
::  to multiply the CHOICE delay by 60.  The "1" at the end
::  of choice can be increased to as high as 99 for 99 minutes.


:START

cls
echo Press "-" to display time now.
echo Press any other letter or number key to exit.
echo.|time|find /i "current"

:LOOP6

:LOOP10
choice /n /c±-1234567890qwertyuiopasdfghjklzxcvbnm /t±,1>nul
if errorlevel 3 goto QUIT
if errorlevel 2 set loop10=*********
if errorlevel 2 set loop6=*****
set loop10=%loop10%*
if not "%loop10%"=="**********" goto LOOP10
set loop10=

set loop6=%loop6%*
if not "%loop6%"=="******" goto LOOP6
set loop6=

goto START

:QUIT
::  http://www.ericphelps.com