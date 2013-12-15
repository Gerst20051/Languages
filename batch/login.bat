    Your problem is finding a way for a logging program 
    to run anytime someone logs in. If you only want it 
    to run when the computer is rebooted, you could 
    reference your batch file in AUTOEXEC.BAT or 
    WINSTART.BAT. If you only want it to run when Windows 
    starts, you might add it to the "run=" line in WIN.INI 
    or put a shortcut to your batch file in the Startup 
    folder for every user. But if you want it to run for 
    every user (including new ones) at every login, you're 
    stuck going into the registry. Press the "Start" 
    button, select "Run...", type in regedit.exe, and 
    navigate your way to 
    HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    Select "Edit" / "New" / "String Value". Name it 
    whatever you want, and add the name of your batch 
    file for the value data.
    
    Now then... Here is the batch file I use for similar 
    things. I call it BOOTLOG.BAT

----------------------------------------------------
call current.bat
echo Login occured at %date% %time% >> c:\bootlog.log
----------------------------------------------------

    The call to CURRENT.BAT is used to set the time and 
    date into the environment. Here is what I use for CURRENT.BAT:

----------------------------------------------------
:: This batch file MUST be named CURRENT.BAT
:: DATE is %4, TIME is %3
:: Current date is Thu 01-22-1998
:: Current time is  8:52:28.07a
if "%3"=="" goto GETDATE
if "%4"=="" goto SETTIME
goto SETDATE
:SETTIME
set TIME=%3
goto DONE
:SETDATE
set DATE=%4
goto DONE
:GETDATE
echo.|date>temp.bat
call temp.bat
echo.|time>temp.bat
call temp.bat
if exist temp.bat del temp.bat
goto DONE
:DONE
----------------------------------------------------

    Someone asked me about combining my batch
    files that identify the user with this batch 
    file. Seemed like a natural! Then I started 
    thinking that all of these batch files create 
    environment variables. And some machines just 
    aren't set up to allow any more variables. So 
    here's code that will log the date and time, 
    will also get the user name three separate 
    ways, and does it all with no variables:

----------------------------------------------------
@echo off
echo ****************************** >> C:\login.log
echo. | date | find "Current" >> C:\login.log
echo. | time | find "Current" >> C:\login.log
net config | find "User" >> C:\login.log
start /w regedit /e C:\temp.txt HKEY_LOCAL_MACHINE\System\CurrentControlSet\control
type C:\temp.txt | find "Current User" >> d:\login\login.log
start /w regedit /e C:\temp.txt HKEY_LOCAL_MACHINE\Network\Logon
type C:\temp.txt | find "username" >> C:\login.log
del C:\temp.txt
echo ****************************** >> C:\login.log
cls
----------------------------------------------------

    If you want your batch file to run "invisibly", 
    well, you have a small problem. Batch files can't 
    run invisibly by themselves. You have to have 
    some other program run start the batch file 
    invisibly. And you have to make sure the program 
    that starts the batch file is invisible itself! 
    sounds like a "Catch-22", but there IS a solution! 
    Windows scripting scripts (VBS files) run 
    invisibly when they are run by the "wscript.exe" 
    program (which is the default way to run them in 
    Windows). So instead of having your registry 
    entry point to your batch file, have it point to 
    a script. Have the script run the batch file. 
    Here's a simple one-line VBS script (give it any 
    name with a VBS file extension) that will run 
    your batch file invisibly. All you have to do is 
    replace the name of the batch file I show with 
    your batch file name:

----------------------------------------------------
CreateObject("Wscript.Shell").Run "C:\login.bat", 0, False
----------------------------------------------------


Miles Fenton in the UK pointed out that while 
"net config" works under Win9x, you'll need to 
use "net config server" or 
"net config workstation" when running under NT.


http://www.ericphelps.com