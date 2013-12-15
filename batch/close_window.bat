FAQ: How do I get my batch file to close it's DOS 
window after it finishes?

Answer: Clear the screen! Just add the following 
two lines to the end of your batch file:
@echo off
cls

It actually makes sense if you think about it. If 
there is something on the screen, DOS will hold 
the window open so you can see the results. But if 
there is NOTHING on the screen, there is no reason 
to keep the window open!

---- Now, before you go and tell me I'm wrong ----

The "official" way (everybody keeps reminding me) 
to close a DOS session is to use the EXIT command. 
But this just closes the DOS session, not 
*necessarily* the window! Even Microsoft admitted 
this in their knowledge base. One problem with 
using EXIT is that if you make DOS batch "utilities" 
that you call from other batch files, an exit 
command can cause your entire program to end without 
returning to the original calling batch file. A 
CLS command, on the other hand, is "Mostly Harmless".

So I keep things separated. If I want to force a 
DOS session to end, I use EXIT. If I just want 
to allow a finished DOS session to close it's 
window, I use CLS. If I want to be safe, I use 
both.

For most people, setting the properties 
for the default command prompt (or creating a 
separate PIF file for each batch file) to close 
the window when completed will solve the problem. 

If you're like me and write batch files that 
are expected to work on more than just *your* 
computer, you'll want to remember to clear the 
screen. It's easier to write good complete code 
than it is to get called out to visit half the 
PCs in your organization to set up PIFs.

http://www.ericphelps.com