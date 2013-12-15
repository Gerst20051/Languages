If you find yourself trying to automate 
the deletion of files, the formatting of 
floppies, or do anything else REALLY 
INTERESTING, you're going to hit a spot 
where everything stops and you get asked 
"Are you sure?". Sometimes you can pass a 
"/y" on the command line (like the COPY 
command) to override this, but that's the 
exception. Often, you can simply ECHO the 
key you'll need to press into the command. 
As an example, suppose you want to run CHKDSK 
with the /F option to fix errors. If it 
finds errors, it will ask you to type "y" 
or "n".  You can create a batch file which 
will do this for you: 

------------
ECHO Y|CHKDSK /F 
------------

If all you needed to do was to press "Enter", 
you could do this: 

------------
ECHO.|CHKDSK /F 
------------

If you need to press "y", AND THEN HIT "Enter", 
it gets more complicated. You have to create 
what is generally called a "script" containing 
the exact keystrokes you need to press. You can 
create it  ahead of time, or as needed like this: 

------------
ECHO Y>SCRIPT.TXT 
ECHO.>>SCRIPT.TXT 
TYPE SCRIPT.TXT|CHKDSK /F 
------------


I use CHKDSK above because it is harmless as an 
example, not because it works. If I told people 
how to format their hard drives, some people would 
type in the code to test it. CHKDSK hasn't been a 
usable command since Windows 95. The technique is 
correct though! 

Use the "pipe" character "|" (the vertical bar) 
to send the output from a command into the input 
of another command. For example:
type test.txt | program.exe
That would send the output of the "type" command 
into the input of the "program.exe" command. The 
"type" command in this case would be putting out 
the contents of the file "test.txt". The 
"program.exe" would (in theory) accept that as 
it's input instead of accepting input from the 
keyboard. 

Use redirection characters ">" and "<" to send 
output between files and programs. Notice the 
difference? The pipe sends stuff between two 
PROGRAMS. Redirection is between a program and 
a FILE. The redirection arrow lets you know what 
direction the data is flowing. For example:
program.exe > test.txt
would take the output of "program.exe" and put 
it in the file "test.txt" INSTEAD of displaying 
it on the screen. The data flows out of the 
program "program.exe" and into the file 
"test.txt". On the other hand:
program.exe < test.txt
Woul cause "program.exe" to use "test.txt" as 
it's input INSTEAD of taking input from the 
keyboard. The dat flows out of the file 
"test.txt" into the program "program.exe".

So these two lines are different ways of doing 
the same thing:
type test.txt | program.exe
program.exe < test.txt
The both end up "program.exe" to use the data in 
"test.txt" for input instead of using the keyboard.

The difference between ">" and ">>" is that ">" 
normally creates a new file, replacing what was 
there, while ">>" just adds to the end of the 
file (If the file doesn't already exist, it will 
be created).

You can even use redirection in non-intuitive 
order and it still works. For example, these two 
lines do the same thing:
program.exe > test.txt
> test.txt program.exe
Looks cute, but it's confusing, huh? Stick with 
the normal way.

So now you know enough to try redirecting or 
piping into the program that stops in DOS and 
asks questions.

HOWEVER -- Sometimes programs will clear the 
keyboard buffer before they ask for user input. 
This has the effect of ignoring everything you 
pipe or redirect into the program! If that's the 
case, you have to fall back to using a separate 
keyboard sinulator program to type the needed keys 
in. Read the first section on this page for a few 
links:
http://www.ericphelps.com/batch/samples/sendkeys.txt



Eric Phelps
http://www.ericphelps.com