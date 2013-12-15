:repeat
set usersname=
for /f "eol=; tokens=1*" %%i in (userlist.txt) do set usersname=%%i
if "%usersname%"=="" goto ender
type userlist.txt| find /v "%usersname%"> userlist1.TXT
copy userlist1.txt userlist.txt
goto repeat
:ender