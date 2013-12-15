  This batch file will repeatedly ping a site until it 
  comes up. You can select how often it will be pinged 
  by changing the number on the 
  choice /c:pq /n /t:p,10 > nul 
  line. Right now I have it set for a 10-second delay. 
  Right now I also have it set to ping 
  "ftp.netscape.com", and you will want to change that 
  to something like MyFriend.dyn.ml.org". Finally, I 
  set it to run NOTEPAD after the ping proves good. 
  You'll want to set it to launch something more useful!
  If you aren't using Winsock2, you may want to update 
  your PING command so it won't hang you. See
  ftp://ftp.microsoft.com/Softlib/MSLFILES/pingupd.exe

--------------
@echo off
echo Press "q" to quit.
:START
choice /c:pq /n /t:p,10 > nul
if errorlevel 2 goto DONE
::   Replace "ftp.netscape.com" with whatever
ping ftp.netscape.com | find "timed out" > nul
if not errorlevel 1 goto START
::   Put whatever command you want to run 
::   AFTER the site comes up here. I use Notepad
::   just as an example.
cls
notepad
:DONE
--------------

  FYI, the CHOICE command is not generally available under NT. 
  You can get it on the disk version of the Resource Kit, but
  not on the download version. If you really want to implement
  this Win9x solution on NT, you can always steal a copy of 
  CHOICE.EXE from a Win9x box. All reports I've heard say it
  works just fine. If a Win98 box isn't handy, download it:
  ftp://ftp.microsoft.com/Services/TechNet/samples/PS/Win98/Reskit/SCRPTING/


  http://www.ericphelps.com