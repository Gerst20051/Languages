@echo off

:: Test to see if we are on Win 9x by how ampersands are handled
> HostsExpired.tmp echo 1234&rem
type HostsExpired.tmp | find "rem" > nul
if errorlevel 1 goto NOT9X
goto ISWIN9X

:NOT9X
if exist HostsExpired.tmp del HostsExpired.tmp
echo This batch file will read the HOSTS file looking for
echo machine names that don't have a DNS "A" entry. A list 
echo of those machines will be appended to a file named
echo "HostsExpired.txt" in the default directory (usually
echo the same directory the batch file is in).

:: Wake the system up by pinging the primary DNS
call :PINGDNS

:: now read the first two words in each line of the HOSTS file
if not exist %windir%\System32\drivers\etc\hosts goto DONE
for /f "tokens=1,2" %%x in (%windir%\System32\drivers\etc\hosts) do call :TESTLINE %%x %%y
goto DONE

:TESTLINE
:: Arguments (dirty) - IP, MachineName
:: Did we get two arguments (was it a blank line)?
if [%2]==[] goto DONE
:: Is it a commented line?
echo %1 | find "#" > nul
if not errorlevel 1 goto DONE
:: Do a NS lookup. If "Address" shows up twice, it is good.
nslookup -type=A %2 2>nul | find /c "Address" | find "2" > nul
if errorlevel 1 call :TESTAGAIN %1 %2
echo %2
goto DONE

:TESTAGAIN
:: Arguments (clean) - IP, MachineName
:: Use ping as a time delay in case NS needed more time to 
:: look up or in case my !@#$%?! DHCP lease expired again.
call :PINGDNS
:: Do a NS lookup. If "Address" shows up twice, it is good.
nslookup -type=A %2 2>nul | find /c "Address" | find "2" > nul
if errorlevel 1 call :LOG %1 %2
goto DONE

:LOG
:: Arguments (clean) - IP, MachineName
:: Add the IP/name entry to the list.
echo %1	%2>> HostsExpired.txt
goto DONE

:PINGDNS
:: Used as a delay or to wake up the system by pinging the primary DNS.
:: Read the ipconfig command and separate things by the colon.
for /f "tokens=1,2 delims=:" %%x in ('ipconfig /all') do call :PINGDNS2 "%%x" %%y

:PINGDNS2
:: Arguments (dirty) - IpconfigEntry, IpconfigValue
:: If %1 (the beginning of the ipconfig line) has "DNS Servers"
:: in it, then %2 (the end of the ipconfig line) has the DNS IP.
echo %1 | find "DNS Servers" > nul
if errorlevel 1 goto DONE
:: Ping the DNS server, but limit the hop count so we *probably* will
:: get out of our local network, but *probably* won't harrass the DNS.
ping -i 4 %2 > nul
ping -i 4 %2 > nul
goto DONE

:ISWIN9X
if exist HostsExpired.tmp del HostsExpired.tmp
echo This batch file requires Windows NT or newer. 
goto DONE

:DONE