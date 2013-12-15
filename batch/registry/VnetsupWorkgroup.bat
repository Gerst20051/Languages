@echo off
start /w regedit /e userdata.reg HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\VxD\VNETSUP
type userdata.reg | find """Workgroup""" > userdata.bat
del userdata.reg > nul
echo set Workgroup=%%1>Workgroup.bat
call userdata.bat
del userdata.bat > nul
del Workgroup.bat > nul
echo Vnetsup Workgroup is %Workgroup%
