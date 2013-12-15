@echo off
start /w regedit /e userdata.reg HKEY_LOCAL_MACHINE\Security\Provider
type userdata.reg | find """Container""" > userdata.bat
del userdata.reg > nul
echo set Container=%%1>Container.bat
call userdata.bat
del userdata.bat > nul
del Container.bat > nul
echo Security Container is %Container%
