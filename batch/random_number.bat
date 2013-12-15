::    Here's a routine that generates a pseudo random number
::    from 0 to 9. It does it by grabbing the hundredths
::    of a second digit from the TIME command. It finishes
::    by setting the environment variable RANDOM.
::
::-----------------------------------------

@echo off
echo.|time|find "Current" >cu##ent.bat
echo set time=%%3> current.bat
call cu##ent.bat
del cu??ent.bat > nul
echo = | choice /c=%time%= temp2.bat > temp1.bat
echo shift>temp2.bat
echo shift>>temp2.bat
echo set RANDOM=%%9>>temp2.bat
call temp1.bat
del temp?.bat > nul

::-----------------------------------------

::  FYI, the CHOICE command is not generally available under NT. 
::  You can get it on the disk version of the Resource Kit, but
::  not on the download version. If you really want to implement
::  this Win9x solution on NT, you can always steal a copy of 
::  CHOICE.EXE from a Win9x box. All reports I've heard say it
::  works just fine. If a Win98 box isn't handy, download it:
::  ftp://ftp.microsoft.com/Services/TechNet/samples/PS/Win98/Reskit/SCRPTING/

::  My favorite NT/2000 guru Joseph Hayes pointed out that
::  under Windows 2000, you can read the %RANDOM% environment
::  variable to get a random number. No need to run a batch 
::  file like described above, because %RANDOM% is built in to 
::  Windows 2000. What amazing progress, huh?

:: http://www.ericphelps.com