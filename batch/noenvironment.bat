@echo off
echo Here is the original environment:
set
echo @echo off > test.bat
path >> test.bat
echo echo. >> test.bat
echo echo Here is the clean environment: >> test.bat
echo set >> test.bat
command noenvironment /c test.bat
del test.bat