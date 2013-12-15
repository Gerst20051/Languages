Want to shut down or log out a PC from a batch file?
Here's a couple of ways:

For those of you with Windows ME, 2000, or newer, you can 
use WMI and Scripting to shut off, log out, or reboot a PC. 
Copy the below text and save it with a VBS file extension:

Const LOGOFF = 0
Const SHUTDOWN = 1
Const REBOOT = 2
Const FORCE = 4
Const POWEROFF = 8
For Each objPC In GetObject("winmgmts:{(shutdown)}").ExecQuery("Select * from Win32_OperatingSystem")
    objPC.Win32Shutdown SHUTDOWN + FORCE
Next

On the line that has "objPC.Win32Shutdown SHUTDOWN + FORCE", 
you can use LOGOFF, SHUTDOWN, REBOOT, or POWEROFF and add 
the "FORCE" value if you don't care about saving open documents.
If you want, you can eliminate all the "Const" lines and use 
numbers instead. For example, instead of using this:
objPC.Win32Shutdown SHUTDOWN + FORCE
you could use this:
objPC.Win32Shutdown 5



Want to shut Win9x from a batch file? Try reading 
this nicely written page:
http://www.winguides.com/registry/display.php/900/


In addition to the methods described there, I've 
found the below items also work on Win95:

rundll user,exitwindows 0
For those of you who are more stealthy, 
you don't have to advertise the function name:

rundll user,#7 0

To do a restart, try one of these:

rundll user,exitwindowsexec
rundll user,#246

And then there are variations like using rundll32 instead
of rundll:

rundll32 user.exe,#7

One of the main hangups is in actually trying to shut 
Windows down from a batch file. The problem is that the
DOS session is still running when Windows tries to shut
down. So you'll often get a box telling you to you have
to shut the program (the DOS box) down. The whole purpose
of the "arguments" I show for "exitwindows" is to force 
the shutdown. I've had a lot more success by using a
scheduling program to execute the rundll program.

Another problem I ran into was screensavers. Some 
screensavers are considered programs and will stop a 
scheduled reboot. The one that bit me was a screensaver 
from the movie Matrix. All day long I tested my reboot 
just fine, but my scheduled 3am reboot always failed until 
I changed the screensaver.

Now then... If you have NT or newer, expect the above 
data to NOT help you! One option is to get hold of the 
resource kit for your version of Windows and look for a 
file called "shutdown.exe" or "logoff.exe":
http://www.microsoft.com/ntserver/nts/downloads/recommended/ntkit/default.asp
http://www.microsoft.com/windows2000/techinfo/reskit/rktour/server/S_tools.asp




http://www.ericphelps.com