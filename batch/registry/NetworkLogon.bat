@echo off
start /w regedit /e reg.txt HKEY_LOCAL_MACHINE\Network\Logon
type reg.txt | find "username" > "us#rname.bat"
echo set NetUser=%%1>"username.bat"
call "us#rname.bat"
del "us?rname.bat" > nul
del reg.txt > nul
echo %NetUser%
exit