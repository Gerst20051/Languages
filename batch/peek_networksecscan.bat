@echo off
>> report.txt echo *******************************************************************************
>> report.txt echo *******************************************************************************
>> report.txt echo *******************************************************************************
>> report.txt echo.
>> report.txt echo.
set /p userdata=Enter notes for this location: 
>> report.txt echo %userdata%
>> report.txt echo %date% %time%
if [%1]==[] goto NOARG
goto GOTARG
:NOARG
>> report.txt echo.
>> report.txt echo ******** IPCONFIG COMMAND ********
>> report.txt echo.
ipconfig /all > ~ip05.txt
>> report.txt type ~ip05.txt
echo FINDING CURRENT IP ADDRESS
ipconfig | find "IP Address" > ~ip01.txt
for /f "tokens=1-2 delims=:" %%i in (~ip01.txt) do echo %%j> ~ip02.txt
for /f "tokens=1-3 delims=." %%i in (~ip02.txt) do echo %%i.%%j.%%k> ~ip03.txt
>> report.txt echo.
>> report.txt echo ******** ROUTE COMMAND ********
>> report.txt echo.
>> report.txt route print
>> report.txt echo.
>> report.txt echo ******** NSLOOKUP COMMAND ********
>> report.txt echo.
>> report.txt nslookup . | find "."
>> report.txt echo.
goto REPORT
:GOTARG
> ~ip02.txt echo %1
for /f "tokens=1-3 delims=." %%i in (~ip02.txt) do echo %%i.%%j.%%k> ~ip03.txt
goto REPORT
:REPORT
>> report.txt echo ******** PING COMMAND ********
>> report.txt echo.
echo PINGING HOSTS IN SUBNET
for /f %%i in (~ip03.txt) do call :PING %%i
>> report.txt type ~ip04.txt
>> report.txt echo.
>> report.txt echo ******** HTTP WEB SERVERS ********
if not exist %0 goto NOHTTP
>> report.txt echo.
echo CHECKING FOR WEB SERVERS
type %0 | find "    " | find /v "not me!" > ~ip00.vbs
for /f %%x in (~ip04.txt) do call :HTTP %%x
type ~ip13.txt
>> report.txt type ~ip13.txt
>> report.txt echo.
:NOHTTP
>> report.txt echo ******** NBTSTAT COMMAND ********
>> report.txt echo.
echo CHECKING NAMES WITH NBTSTAT
for /f %%x in (~ip04.txt) do echo %%x & echo %%x >> report.txt & nbtstat -A %%x | find "Registered" >> report.txt && echo %%x>> ~ip11.txt
>> report.txt echo.
>> report.txt echo ******** NET USE COMMAND ********
>> report.txt echo.
echo CHECKING SHARES WITH NET USE
for /f %%x in (~ip04.txt) do call :NETUSE %%x
net use | find "\\" > ~ip10.txt
>> report.txt type ~ip10.txt
>> report.txt echo.
>> report.txt echo ******** NET VIEW COMMAND ********
>> report.txt echo.
echo LISTING SHARED RESOURCES WITH NET VIEW
net view | find "\\" > ~ip08.txt
type ~ip08.txt
>> report.txt echo ----------
>> report.txt type ~ip08.txt
>> report.txt echo ----------
>> report.txt echo.
if exist ~ip11.txt for /f %%x in (~ip11.txt) do call :NETVIEW %%x
>> report.txt echo.
>> report.txt echo ******** ARP COMMAND ********
>> report.txt echo.
>> report.txt arp -a | find "-" | find /v "invalid"
>> report.txt echo %date% %time%
>> report.txt echo.
>> report.txt echo.
>> report.txt echo *******************************************************************************
>> report.txt echo *******************************************************************************
>> report.txt echo *******************************************************************************

If not exist ~ip06.txt goto NOSHARE
set /p userdata=View remote shares (y/n)? 
if [%userdata%]==[y] for /f %%x in (~ip06.txt) do start \\%%x
:NOSHARE
if not exist ~ip13.txt goto NOWEB
set /p userdata=View web sites (y/n)? 
if [%userdata%]==[y] for /f %%x in (~ip13.txt) do start http://%%x
:NOWEB

:CLEANUP
echo y>~ip09.txt
type ~ip09.txt | net use * /delete
del ~ip??.???
goto :EOF

:NETUSE
net use \\%1\IPC$ "" /USER:"" | find "success"> ~ip12.txt
if not errorlevel 1 echo %1> ~ip06.txt
goto :EOF

:HTTP
cscript.exe ~ip00.vbs %1 >> ~ip13.txt
goto :EOF
    On Error Resume Next
    Set oHttp = CreateObject("WinHttp.WinHttpRequest.5.1")
    oHttp.SetTimeouts 1000, 1000, 1000, 1000
    oHttp.Open "GET", "http://" & WScript.Arguments(0), False
    oHttp.Send
    If err.number = 0 Then Wscript.Echo WScript.Arguments(0) & "    "  & oHttp.Status
    Set oHttp = Nothing

:NETVIEW
net view \\%1 > ~ip07.txt
type ~ip07.txt | find "\\"
type ~ip07.txt | find "Disk"
type ~ip07.txt | find "\\" >> report.txt
type ~ip07.txt | find "Disk" >> report.txt
goto :EOF

:PING
for /L %%x in (1,1,254) do echo %1.%%x & ping -n 1 -w 50 -i 6 %1.%%x | find "Reply" && echo %1.%%x>> ~ip04.txt
goto :EOF
