@echo off

:: Test to see if we are on Win 9x by how ampersands are handled
> HostsRemove.tmp echo 1234&rem
type HostsRemove.tmp | find "rem" > nul
if errorlevel 1 goto START
goto WIN9X


:START
if exist HostsRemove.tmp del HostsRemove.tmp
:: Did we get any arguments?
if [%1]==[] goto NOARGS
if not exist "%1" goto BADFILE
:: Can we see the HOSTS file?
if not exist %windir%\System32\drivers\etc\hosts goto NOHOSTS
:: Verify this is what the user really wants
echo This batch file will HELP you remove entries from your 
echo HOSTS file. A NEW file will be created that will be a 
echo copy of your existing HOSTS file EXCEPT that it will 
echo not contain any of the machine entries found in the 
echo "%1" file you supplied. 
echo YOUR EXISTING HOSTS FILE WILL NOT BE MODIFIED.
echo After this batch file runs, the new "cleaner" HOSTS 
echo file will be available as "HostsRemove.txt" in the 
echo default directory (generally the same directory the 
echo batch file is in). It will be up to you to update your 
echo HOSTS file with the new data.
echo.
pause
:: Create the copy of the HOSTS file
type %windir%\System32\drivers\etc\hosts > HostsRemove.txt
:: Get the short file name -- We need an unquoted file name for list processing.
for %%x in ("%1") do call :READFILE %%~fsx
goto DONE

:READFILE
:: Arguments (dirty) - Fully qualified short file name
:: Be sure the short name we got points to a real file
if [%1]==[] goto BADFILE
if not exist %1 goto BADFILE
:: Set up a FOR loop to read every line in the file
for /f "tokens=1,2" %%x in (%1) do call :TESTLINE %%x %%y
if exist HostsRemove.tmp del HostsRemove.tmp
goto DONE

:TESTLINE
:: Arguments (dirty) - IP, MachineName
:: Did we get two arguments (was it a blank line)?
if [%2]==[] goto DONE
:: Is it a commented line?
echo %1 | find "#" > nul
if not errorlevel 1 goto DONE
:: Do the IP and machine both have dots?
echo %1 | find "." > nul
if errorlevel 1 goto DONE
echo %2 | find "." > nul
if errorlevel 1 goto DONE
call :EDITHOSTS %2
goto DONE

:EDITHOSTS
:: Arguments (clean) - MachineName
echo %1
:: Remove machine name preceeded by a space
type HostsRemove.txt | find /v " %1" > HostsRemove.tmp
type HostsRemove.tmp > HostsRemove.txt
:: Remove machine name preceeded by a tab
type HostsRemove.txt | find /v "	%1" > HostsRemove.tmp
type HostsRemove.tmp > HostsRemove.txt
goto DONE

:NOHOSTS
echo Sorry, but your HOSTS file isn't where I expected it.
echo You're going to have to either create a HOSTS file or 
echo edit this batch file to point to wherever your HOSTS 
echo file is.
pause
goto DONE

:BADFILE
:: Arguments -- None. However, %1 should be available.
echo The argument you supplied, "%1", doesn't seem to be a valid file name.
pause
goto DONE

:NOARGS
echo Drop a file on me! The file should contain IP adresses
echo and machine names in the same format as a HOSTS file.
echo For example:
echo.
echo 127.0.0.1	www.viruses-to-go.com
echo 127.0.0.1	www.sell-your-name.com
echo.
pause
goto DONE

:WIN9X
if exist HostsExpired.tmp del HostsExpired.tmp
echo This batch file requires Windows NT or newer. 
pause
goto DONE

:DONE