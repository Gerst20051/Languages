I want to automate my login, but it always 
stops at the part where I have to enter my 
password.

The solution to this depends on how your 
login command works. First try typing 
login /? 
and see if it is possible to specify the 
password on the command line. No decent 
system should allow it! 

Next, try creating a file with EXACTLY the 
keystrokes you need to enter in order to 
log in. On my system (using Netware 4), we 
can specify server and user name on the 
command line. All I have to do is enter 
my password and hit "Enter". So the first 
thing I do is create a file named "login.txt" 
with nothing but my password and a carriage 
return:

---------
PaSsWoRd

---------

Then I create the batch file which logs me in. 
It "pipes" the login.txt file into the actual 
login command. Basically, this forces the login 
command to take it's input from the login.txt 
file INSTEAD of from the keyboard:

---------
type login.txt | login sanwfs1/ericp
---------

In the above example, sanwfs1 is my server, 
and ericp is my user name. Now, if something 
goes wrong with your login (say it tells you 
your password expired and asks you for a new 
one), you are screwed. Nothing you do to the 
keyboard will work because the login command 
ONLY looks at what you've typed into the 
login.txt file. So you better type everything 
you think you MIGHT need in there. You might 
want to put a couple "N" characters in there 
to answer "No" to any questions. Frankly, no 
decent system should allow you to automate 
your login! Any GOOD system will AT A MINIMUM 
clear the keyboard buffer and flush STDIN to 
force password entry from the actual keyboard. 

Because your problem won't be as easy to solve 
as the above suggestion, you might also want to 
take a look at the "sendkeys.txt" file in this 
same directory for more suggestions.

http://www.ericphelps.com
