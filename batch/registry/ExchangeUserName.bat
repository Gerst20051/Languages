@echo off
start /w regedit /e userdata.reg HKEY_CURRENT_USER\Software\Microsoft\Exchange
type userdata.reg | find """UserName""" > userdata.bat
del userdata.reg > nul
echo set UserName=%%1>UserName.bat
call userdata.bat
del userdata.bat > nul
del UserName.bat > nul
echo Exchange UserName is %UserName%
