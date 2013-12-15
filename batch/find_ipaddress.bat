To find your IP address in a batch file, you must 
first find your IP address and get it to appear on 
a line:


Under NT (and some Win9x machines) you can run the 
IPCONFIG program and parse the results:

ipconfig.exe | find "IP Address" | find /v " 0.0.0.0"

Windows 95 and 98 have a different program 
WINIPCFG. Here's how to do it in Win95:

winipcfg.exe /batch %temp%\winipcfg.out
type %temp%\winipcfg.out | find "IP Address" | find /v " 0.0.0.0"

Another possibility that works on both types 
of machines is to use ARP:

ping.exe -n 1 -i 1 -w 1 www.microsoft.com
arp.exe -a | find "Interface"

   If you have NT, let me stop you right here. I do NOT 
   write NT batch code, but I slapped this little bit 
   together. This will put your NT IP address into 
   a file named "ip3.txt":

   ipconfig | find "IP Address" > ip1.txt
   for /f "tokens=1-2 delims=:" %i in (ip1.txt) do echo %j> ip2.txt
   for /f "tokens=1-3 delims=." %i in (ip2.txt) do echo %i.%j.%k.21> ip3.txt
   
   You'll find that Win9x solutions will be very moch 
   more complicated because Win9x doesn't have the powerful 
   NT "for" command options.

Now, on to those complicated Win9x solutions...
Now that you have the IP on a line, you have to 
extract it from all the other words on the line. 
This is easily done by constructing two other files. 
One of these files is a "line fragment" (a single 
line with NO line termination characters), while 
the other file is a separate batch file. In this case, 
I'll use a batch file named "temp2.bat", so I'll 
need a line fragment that has "call temp2.bat " 
(without the quotes but WITH the trailing space) in 
it. While you could make the fragment ahead of time 
(see http://www.ericphelps.com/batch/lines/frag-man.htm) 
I'll show code here which will make the fragment as 
needed. The fragment will be called "temp.txt":


:: Make a line fragment
:: See http://www.ericphelps.com/batch/lines/frag-dbg.htm
echo e 100 "call temp2.bat "> script
echo rcx>> script
echo f>> script
echo n temp.txt>> script
echo w>> script
echo q>>script
debug < script > junk
del script
del junk

Assuming we have the fragment, we need to put the IP 
data on the end of the fragment and rename it as a batch 
file. Using the NT example above (because it's the shortest):

copy temp.txt temp1.bat > junk
ipconfig.exe | find "IP Address" | find /v " 0.0.0.0" >> temp1.bat

I redirect the output of commands whose outputs I don't 
want to see into "junk". Under Win9x, I could have redirected 
into "nul", but that just creates a file under NT. So I opt 
for uniformity and let a file named "junk" get created. I'll 
delete it later.

Let's recap. Right now we have one pice of code that makes 
the fragment. Another piece puts the IP on the fragment as 
"temp1.bat". So let's peek at what is typically in "temp1.bat":

call temp2.bat	IP Address. . . . . . . . . : 147.132.1.151

Obviously, when we run temp1.bat, it's going to try to call 
something called temp2.bat. And it will pass the rest of the 
line as arguments. The first argument is "IP". the second is 
"Address.", all the way to the twelfth argument which is 
"147.132.1.151". So our temp2.bat will be in the unique 
position of being able to isolate the IP address. 
Unfortunately, we can't access variables higher than 9, so 
we'll have to shift three times to move 12 down to 9. 
Here's what could be in temp2.bat:

shift
shift
shift
set IP=%9

We can either pre-build our temp2.bat, or we can create it 
like this:

echo shift> temp2.bat
echo shift>> temp2.bat
echo shift>> temp2.bat
echo set %%9>> temp2.bat

If we pre-build everything, we'll need the main batch file, 
the fragment (temp.txt), and the working file (temp2.bat). 
But I think it's simpler to go for a single file solution. 
Here is all the above sample code together in one file:

@echo off
:: Make a line fragment "temp.txt"
echo e 100 "call temp2.bat "> script
echo rcx>> script
echo f>> script
echo n temp.txt>> script
echo w>> script
echo q>>script
debug < script > junk
del script
:: Make the working file "temp2.bat"
echo shift> temp2.bat
echo shift>> temp2.bat
echo shift>> temp2.bat
echo set IP=%%9>> temp2.bat
:: Run the command that finds the IP and create "temp1.bat"
copy temp.txt temp1.bat > junk
ipconfig.exe | find "IP Address" | find /v " 0.0.0.0" >> temp1.bat
:: Run the temp1.bat, which runs temp2.bat, which sets the IP variable
call temp1.bat
:: Remove temporary files
del temp1.bat
del temp2.bat
del temp.txt
del junk
:: Display the result
echo Your IP is %IP%

As usual, no guarantee that the above code will work on 
NT because I tested it under Win95. Here's the same batch 
file modified for the Windows 9x "winipcfg" program:

@echo off
:: Make a line fragment "temp.txt"
echo e 100 "call temp2.bat "> script
echo rcx>> script
echo f>> script
echo n temp.txt>> script
echo w>> script
echo q>>script
debug < script > junk
del script
:: Make the working file "temp2.bat"
echo shift> temp2.bat
echo shift>> temp2.bat
echo shift>> temp2.bat
echo set IP=%%9>> temp2.bat
:: Run the command that finds the IP and create "temp1.bat"
copy temp.txt temp1.bat > junk
winipcfg.exe /batch %temp%\winipcfg.out
type %temp%\winipcfg.out | find "IP Address" | find /v " 0.0.0.0" >> temp1.bat
:: Run the temp1.bat, which runs temp2.bat, which sets the IP variable
call temp1.bat
:: Remove temporary files
del temp1.bat
del temp2.bat
del temp.txt
del junk
del %temp%\winipcfg.out
:: Display the result
echo Your IP is %IP%

As you can see, exactly the same except for the two 
lines needed for winipcfg. Now here it is modified to 
use the ARP program:

@echo off
:: Make a line fragment "temp.txt"
echo e 100 "call temp2.bat "> script
echo rcx>> script
echo f>> script
echo n temp.txt>> script
echo w>> script
echo q>>script
debug < script > junk
del script
:: Make the working file "temp2.bat"
echo set IP=%%2> temp2.bat
:: Run the command that finds the IP and create "temp1.bat"
copy temp.txt temp1.bat > junk
ping.exe -n 1 -i 1 -w 1 www.microsoft.com > junk
arp.exe -a | find "Interface" >> temp1.bat
:: Run the temp1.bat, which runs temp2.bat, which sets the IP variable
call temp1.bat
:: Remove temporary files
del temp1.bat
del temp2.bat
del temp.txt
del junk
:: Display the result
echo Your IP is %IP%

Finally, you may get an error about being out of 
environment space whenever you try to run a batch 
file that sets variables. The easiest solution is 
to not set the variable! For example, the temp2.bat 
could be modified from this:

echo set IP=%%2> temp2.bat

to this:

echo echo Your IP is %%2> temp2.bat

Of course, you'd substitute %%9 for the %%2 I show if 
you used the first two (IPCONFIG or WINIPCFG) 
examples. You also wouldn't need the last two lines 
in the code:

:: Display the result
echo Your IP is %IP%

because the display would be handled in temp2.bat.