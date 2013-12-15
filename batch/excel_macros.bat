@echo off
::set the filename line below to whatever Excel document it is you want to open
if [%1]==[] set filename=c:\my documents\book1.xls
if not [%1]==[] set filename=%1
if [%windir%]==[] goto DONE
:: Set Excel 97 macro virus protection off.
 >userdata.reg echo REGEDIT4
>>userdata.reg echo.
>>userdata.reg echo [HKEY_CURRENT_USER\Software\Microsoft\Office\8.0\Excel\Microsoft Excel]
>>userdata.reg echo "Options6"=dword:00000000
>>userdata.reg echo.
start /w regedit /s userdata.reg
del userdata.reg > nul
start /w "%filename%"
set filename=
:: Set Excel 97 macro virus protection on.
 >userdata.reg echo REGEDIT4
>>userdata.reg echo.
>>userdata.reg echo [HKEY_CURRENT_USER\Software\Microsoft\Office\8.0\Excel\Microsoft Excel]
>>userdata.reg echo "Options6"=dword:00000008
>>userdata.reg echo.
start /w regedit /s userdata.reg
del userdata.reg > nul
:DONE
:: Clear the screen to close this window
cls