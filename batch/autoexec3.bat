@echo off
prompt $P$G
PATH=C:\DOS;C:\WINDOWS
set TEMP=C:\TEMP
set BLASTER=A220 I7 D1 T2
goto %CONFIG%
:WIN
 lh smartdrv.exe
 lh mouse.com /Y
 win
goto END
:XMS
 lh smartdrv.exe
 lh doskey
 goto END
:END

REM The goto %CONFIG% line informs DOS to look up menu entries
:: that were defined within CONFIG.SYS.