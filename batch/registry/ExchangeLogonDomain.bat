@echo off
start /w regedit /e userdata.reg HKEY_CURRENT_USER\Software\Microsoft\Exchange
type userdata.reg | find """LogonDomain""" > userdata.bat
del userdata.reg > nul
echo set LogonDomain=%%1>LogonDomain.bat
call userdata.bat
del userdata.bat > nul
del LogonDomain.bat > nul
echo Exchange LogonDomain is %LogonDomain%
