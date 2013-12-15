@echo off
start /w regedit /e userdata.reg HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\VxD\VNETSUP
type userdata.reg | find """ComputerName""" > userdata.bat
del userdata.reg > nul
echo set ComputerName=%%1>ComputerName.bat
call userdata.bat
del userdata.bat > nul
del ComputerName.bat > nul
echo Vnetsup ComputerName is %ComputerName%
