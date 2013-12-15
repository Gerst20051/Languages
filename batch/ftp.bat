Get yourself a DOS prompt, type FTP and hit Enter. You've found 
the built-in command-line FTP program that ships with every copy 
of Windows 95 and NT! That means it's the only thing you KNOW will 
be on someone else's computer. But talking someone through how to 
use FTP is messy. If you need to help someone else out, you're 
better off automating the whole process by supplying them with an 
FTP "script". If you need to learn more about FTP, read on and go 
photocopy the FTP section out of a UNIX or LINUX book.

Before you go further, the sample FTP command lines I give here are 
(like most of my code) for Win9x. If you have NT, don't leave this 
page without checking out the NT difference mentioned at the bottom 
of this page!

FTP supports scripting. You use the "-s:" option to specify a script, 
for example if you name this text SCRIPT.TXT:

open ftp.microsoft.com
anonymous
username@nowhere.com
cd Products
cd Windows
cd Windows95
cd CDRomExtras
cd AdministrationTools
cd ApplicationTools
binary
hash
lcd c:\
get envars.exe
bye

Then you could execute it like this:

ftp -s:SCRIPT.TXT

There is one complication. Some people have more than one 
FTP program. So we need to specify we want to use the original. 
Do it by replacing "ftp" in the above example with %windir%\ftp.exe
for example

%windir%\ftp.exe -s:SCRIPT.TXT

By the way, the "envars.exe" the example script above downloads 
is a zipped file that contains two programs. One allows you to 
create shortcuts from DOS, and the other allows you to set 
system environment variables. Both are for Win9x only.

Here are all the command-line options for FTP:
FTP [-v] [-d] [-i] [-n] [-g] [-s:filename] [-a] [-w:buffer] [-A] [host]

  -v           Suppresses display of remote server responses.
  -n           Suppresses auto-login upon initial connection.
  -i           Turns off interactive prompting during multiple file
               transfers.
  -d           Enables debugging.
  -g           Disables filename globbing (see GLOB command).
  -s:filename  Specifies a text file containing FTP commands; the
               commands will automatically run after FTP starts.
  -a           Use any local interface when binding data connection.
  -A           login as anonymous.
  -w:buffer    Overrides the default transfer buffer size of 4096.
  host         Specifies the host name or IP address of the remote
               host to connect to.

Using the "-A" option logs you in with user and host name you 
specified in your TCPIP setup. Which is why I prefer to specify 
the login name and password in the script.

Here are all the local FTP commands (which you can see by 
typing "help" at an FTP prompt:

!           delete       literal     prompt       send
?           debug        ls          put          status
append      dir          mdelete     pwd          trace
ascii       disconnect   mdir        quit         type
bell        get          mget        quote        user
binary      glob         mkdir       recv         verbose
bye         hash         mls         remotehelp
cd          help         mput        rename
close       lcd          open        rmdir

Just type help (or ?) followed by the command you want a 
description of. For example:
help append
gives this response:
append          Append to a file
Okay, it isn't much. But it's a start. 

You can even turn your script into a batch file:

%windir%\ftp.exe -s:%0
goto done
open ftp.microsoft.com
anonymous
username@nowhere.com
cd Products
cd Windows
cd Windows95
cd CDRomExtras
cd AdministrationTools
cd ApplicationTools
binary
hash
lcd c:\
get envars.exe
bye
:done
@echo off
cls
exit

I only had to add two lines to the beginning of my script 
and four lines to the end to turn it into a batch file. The 
great part is those same lines will turn ANY FTP script into 
a batch file! The DOS batch file will jump over the ftp 
script part, and the FTP program will just return "Invalid 
command" and go on to the next line harmlessly when it hits 
the DOS batch file commands.

But remember -- this and all files I write are for Win95! If
you find yourself using NT/W2K, you'll need to change the 
%windir%\ftp.exe -s:%0
line in the above code to
%windir%\system32\ftp.exe -s:%~f0 
(or %windir%\system32\ftp.exe -s:"%~f0" if the batch file path has embedded spaces)
NT is different in small ways that make big problems.
(Thanks to Joseph Hayes for pointing that one out!)
Also, if you still have problems on NT, you may need to add 
your "account" to your password in the script file. See here 
for more information:
http://support.microsoft.com/default.aspx?scid=kb;EN-US;q135858

Now then... Let's assume you don't want to embed your user name 
and password in the FTP batch file or script. Or maybe you want 
to pull the user name from somewhere else. Same problem. You're 
going to have to have your batch file create a separate script 
as it is needed. We'll start with a simple modification of the 
above sample code:

@echo off
>  C:\script.txt echo open ftp.microsoft.com
>> C:\script.txt echo anonymous
>> C:\script.txt echo username@nowhere.com
>> C:\script.txt echo cd Products
>> C:\script.txt echo cd Windows
>> C:\script.txt echo cd Windows95
>> C:\script.txt echo cd CDRomExtras
>> C:\script.txt echo cd AdministrationTools
>> C:\script.txt echo cd ApplicationTools
>> C:\script.txt echo binary
>> C:\script.txt echo hash
>> C:\script.txt echo lcd c:\
>> C:\script.txt echo get envars.exe
>> C:\script.txt echo bye
start /w %windir%\ftp.exe -s:C:\script.txt
del C:\script.txt
cls
exit

Those of you "new" to batch may be confused over the 
"> file echo text" layout. Maybe you are used to 
"echo text > file". They both work the same. The only reason 
I used the unusual layout is that it looks neater and is 
easier to copy and paste.

Now, suppose you had wanted batch file that could be called 
with the user name as parameter one, and the password as 
parameter two. You'd just modify the above code like this:

@echo off
>  C:\script.txt echo open ftp.microsoft.com
>> C:\script.txt echo %1
>> C:\script.txt echo %2
>> C:\script.txt echo cd Products
... and so on

Or maybe you have the user name and password stored in the 
environment as %user% and %pass%. You would do this:

@echo off
>  C:\script.txt echo open ftp.microsoft.com
>> C:\script.txt echo %user%
>> C:\script.txt echo %pass%
>> C:\script.txt echo cd Products
... and so on

Notice at the end of this batch file, I run FTP by using the 
START command with the /W (wait) option. That's so we don't 
delete the script we made until FTP is done using it! Under NT, 
sometimes (quite often) NT won't wait on a program even if you 
specify /w. In those cases, you may have to specify other 
options for START and force FTP to run in the same memory space 
as your batch file.

If you want a few ways to prompt a user for user names and 
passwords, check out the sample code on this page:
http://www.ericphelps.com/batch/samples/samples.htm
or read about it in general here:
http://www.ericphelps.com/batch/userin/index.htm

If you want to embed your user name in your batch file but don't 
want it visible, read my advice here:
http://www.ericphelps.com/batch/samples/obfuscating.txt



::  http://www.ericphelps.com