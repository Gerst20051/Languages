@echo off
start /w regedit /e userdata.reg HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion
type userdata.reg | find """RegisteredOwner""" > userdata.bat
del userdata.reg > nul
echo set RegisteredOwner=%%1>RegisteredOwner.bat
call userdata.bat
del userdata.bat > nul
del RegisteredOwner.bat > nul
echo Windows RegisteredOwner is %RegisteredOwner%
