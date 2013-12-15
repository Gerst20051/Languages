@echo off
color 3
title Conditional Shutdown
set /p name=enter a name:
:start 
cls
echo Hi, %name%
echo. 
echo 1.Shutdown
echo 2.Quit
set /p choice=enter your choice 1,2: 
if %choice%==1 goto shutdown
if %choice%==2 exit
 
:shutdown
cls
set /p sec=enter the number of seconds that you wish the computer to shutdown in: 
set /p message=enter the shutdown message you wish to display: 
shutdown -s -f -t %sec% -c "%message%"
echo shutdown initiated at %time%
set /p cancel=type cancel to stop shutdown 
if %cancel%==cancel shutdown -a
if %cancel%==cancel goto start