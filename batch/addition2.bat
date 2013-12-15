  This example illustrates addition in batch 
  files. Notice this isn't done by real math, 
  but by text comparisons. A common use for an 
  addition routine would be as a loop counter. 
  Here is a batch file that could ping a 
  subnet looking for computers. Note 
  that this example uses the ADD.BAT shown 
  further below. This example also assumes 
  that PING will deliver a response with the 
  word "Reply" in it ONLY if it succeeds (which 
  is true for Winsock 1 and 2 under Win95):

----------------------------------
@echo off
:START
call add.bat
ping 127.0.0.%H%%T%%D% | find "Reply" > nul
if not errorlevel 1 echo %H%%T%%D%
if %H%%T%%D%==254 goto DONE
goto START
:DONE
----------------------------------

  Notice that the above example is really a 
  simple loop running from 1 to 254 with the 
  "ping" and "if not" lines inside the loop. 
  By pre-setting values for %H%, %T%, and %D% 
  or by changing the "==254" value, you can 
  easily set the start and ending values of 
  the loop.

  If you want a list of the good numbers 
  (instead of just an onscreen display), 
  make another batch file called 
  pinglist.bat with this one line in it:
echo %1 >> goodpings.txt
  and change the line that reads
if not errorlevel 1 echo %H%%T%%D%
  in the example above to
if not errorlevel 1 call pinglist.bat %H%%T%%D%
  Here is the ADD.BAT used in the above example:

----------------------------------
:: ADD.BAT
:: Increments a three digit number
:: Works by comparing each digit
:: H=hundreds, T=tens, D=digits
@echo off
if [%H%]==[] set H=0
if [%T%]==[] set T=0
if [%D%]==[] set D=0
:DIGITS
if %D%==9 goto TENS
if %D%==8 set D=9
if %D%==7 set D=8
if %D%==6 set D=7
if %D%==5 set D=6
if %D%==4 set D=5
if %D%==3 set D=4
if %D%==2 set D=3
if %D%==1 set D=2
if %D%==0 set D=1
goto DONE
:TENS
set D=0
if %T%==9 goto HUNDREDS
if %T%==8 set T=9
if %T%==7 set T=8
if %T%==6 set T=7
if %T%==5 set T=6
if %T%==4 set T=5
if %T%==3 set T=4
if %T%==2 set T=3
if %T%==1 set T=2
if %T%==0 set T=1
goto DONE
:HUNDREDS
set T=0
if %H%==9 set H=0
if %H%==8 set H=9
if %H%==7 set H=8
if %H%==6 set H=7
if %H%==5 set H=6
if %H%==4 set H=5
if %H%==3 set H=4
if %H%==2 set H=3
if %H%==1 set H=2
if %H%==0 set H=1
goto DONE
:DONE
----------------------------------

::  http://www.ericphelps.com
