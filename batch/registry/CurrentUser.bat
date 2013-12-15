@echo off
start /w regedit /e reg.txt HKEY_LOCAL_MACHINE\System\CurrentControlSet\control
type reg.txt | find "Current User" > "Current#User.bat"
echo set CurrentUser=%%1>"Current User.bat"
call "Current#User.bat"
del "Current?User.bat" > nul
del reg.txt > nul
echo %CurrentUser%
exit