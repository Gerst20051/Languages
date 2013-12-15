:: Here's NT code that puts something like 
:: 03/17/2002
:: into the DATE environment variable
@echo off
echo.|date|find "current" >t#e.bat
echo set date=%%5> the.bat
call t#e.bat
del t?e.bat > nul
:: Thanks to Joseph P. Hayes
:: joephayes@yahoo.com


:: Here's a one-line NT command (but a LONG line!) 
:: one that splits all the date parts out, putting
:: each one into it's own variable (weekday, day, month, year)
for /f "tokens=1-4 delims=/ " %%a in ('date /t') do (set weekday=%%a& set day=%%b& set month=%%c& set year=%%d)


:: And here's NT code that puts the time into the environment
for /f %%a in ('time /t') do set time=%%a


:: Here's a simple way to see if today is Monday (or any other day)
date /t | find "Mon"
if not errorlevel 1 goto MONDAY
goto OTHERDAY