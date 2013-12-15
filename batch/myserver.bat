@echo off
:: This batch file was built on Win95 and NT, so probably works
:: on everything. Run it with no arguments to see built-in help.
:: It depends heavily on the commands DEBUG, PING, and ARP.
:: It generates lots of temporay files all with names like "~temp"

:: Check for argument
if [%1]==[] goto NOARG
goto GOODARG
:NOARG
cls
echo Pass this batch file a number from 1 to 254 and it will set an
echo environment variable named IP with your IP number except with 
echo the number you supplied on the end. For example, if your IP is
echo 127.0.0.1 and you do this:
echo %0 34
echo You'd get the IP variable set to 127.0.0.34
goto DONE
:GOODARG

:: Check environment space
set ip=127.000.000.001--------
set pos=00
If [%ip%%pos%]==[127.000.000.001--------00] goto GOODENV
goto BADENV
:BADENV
cls
echo Sorry, but your computer doesn't have enough environment
echo space to hold the variables this program needs to run.
goto DONE
:GOODENV

echo e 100 "call ~temp2.bat "> ~temp1.txt
echo rcx>> ~temp1.txt
echo 10>> ~temp1.txt
echo n ~temp1.bat>> ~temp1.txt
echo w>> ~temp1.txt
echo q>>~temp1.txt
debug < ~temp1.txt > ~temp2.txt
del ~temp1.txt
del ~temp2.txt

:: Get the IP of the local machine by using ARP (same on 9x and NT)
ping.exe -n 1 -i 1 -w 1 www.microsoft.com > ~temp0.txt
arp.exe -a | find "Interface">> ~temp1.bat
:: "~temp1.bat" now contains "call ~temp2.bat Interface: 10.134.1.207 on Interface 2"
echo set ip=%%2> ~temp2.bat
call ~temp1.bat
del ~temp0.txt
del ~temp1.bat
del ~temp2.bat
:: We now have the IP in the environment.

:: Save the IP to "~temp3.bat" complete with a new ip set command
:: The dashes are necessary space fillers to prevent random data 
:: in memory locations we will be testing in the next step
echo set ip=%ip%--------> ~temp3.bat

:: Now locate the position of the three dots with DEBUG
echo S 100 L 13, 2E> ~temp3.txt
echo Q>> ~temp3.txt
debug ~temp3.bat < ~temp3.txt | find ":" > ~temp4.txt
del ~temp3.txt

:: Read the output of DEBUG to put the position in the environment
:: "~temp4.txt" contains three lines like this: "0FC0:010F"
:: First four digits are ignored, then ":01". We are only
:: interested in last two digits (position of dot).
:: Position of last dot is somewhere between 0C and 12
:: where the letter "s" is 00, letter "e" is 01, and so on.
:: set ip=0.0.0.0 --- last dot is in position 0C (hex)
:: set ip=000.000.000.000 -- last dot is in position 12 (hex)
type ~temp4.txt | find ":010C" > ~temp5.txt
if not errorlevel 1 set pos=0C
type ~temp4.txt | find ":010D" > ~temp5.txt
if not errorlevel 1 set pos=0D
type ~temp4.txt | find ":010E" > ~temp5.txt
if not errorlevel 1 set pos=0E
type ~temp4.txt | find ":010F" > ~temp5.txt
if not errorlevel 1 set pos=0F
type ~temp4.txt | find ":0110" > ~temp5.txt
if not errorlevel 1 set pos=10
type ~temp4.txt | find ":0111" > ~temp5.txt
if not errorlevel 1 set pos=11
type ~temp4.txt | find ":0112" > ~temp5.txt
if not errorlevel 1 set pos=12
del ~temp4.txt
del ~temp5.txt

:: Now chop the ip in the file off at the last dot
echo RCX> ~temp6.txt
echo %POS%>> ~temp6.txt
echo W>> ~temp6.txt
echo Q>> ~temp6.txt
debug ~temp3.bat < ~temp6.txt > ~temp7.txt
del ~temp6.txt
del ~temp7.txt
set pos=

::Now add the machine IP to the "~temp.bat" file
echo .%1>> ~temp3.bat

:: Now run the "~temp3.bat" which will set %IP% to the desired server
call ~temp3.bat
del ~temp3.bat

:DONE