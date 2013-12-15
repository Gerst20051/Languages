    How to do a delay.
    I used to think it couldn't be done. Then I thought it 
    can't be done WELL! But I know several ways to do it. 
    Each with it's own problems. But first a cautionary note: 
    Most people who THINK they need a time delay actually want 
    to give some other program time to finish. That is best 
    accomplished with the START command using the /w option: 

START /W MYPROGRAM.EXE
    _______________________________________________________

    The first way to make a time delay uses the CHOICE 
    command. CHOICE has the "/t" option which lets it 
    automatically select for you after a time delay. Try 
    this in a DOS window now:

choice /ty,10

    If you do NOTHING, it will wait ten seconds and enter a 
    "y" for you. The problem is that if you hit anything 
    other than the "y" or "n" key in those ten seconds, it 
    will stop the timer and wait FOREVER for you to hit the 
    correct key (a "y" or an "n" in this case). On the other 
    hand, if you hit a correct key in those ten seconds, it 
    stops the timer and continues IMMEDIATELY. Now generally, 
    when you code for ten seconds, you want ten seconds. Not 
    immediately, and not forever. If you can keep the end 
    user's hands off the keyboard, things work just fine.
    _______________________________________________________

    Now, there is approach that almost eliminates the keyboard 
    problems. You can force CHOICE to ignore the keyboard by 
    redirecting it's input like this:

rem | choice /ty,10

    Nothing you do will stop or hang the time delay. But keys 
    entered WILL get passed on to whatever happens next. Just 
    keep it in mind. And remember you can use the /n and > nul
    tricks to keep your screen clean:

rem | choice /n /ty,10 > nul

    _______________________________________________________

    There is another workaround to the keyboard problem: Make 
    all keys acceptable:

choice /n /c±-1234567890qwertyuiopasdfghjklzxcvbnm /t±,10 > nul
if not errorlevel 1 echo You hit a key and aborted the timeout!

    Okay, I did several things at once in the above code. 
    The ± character is one of the "extended" characters (177) 
    that nobody is likely to enter from a keyboard. I made 
    that extended character the default character. I also 
    made it the first character. That way I know if I get 
    an errorlevel that isn't a one, some other key was entered. 
    And I made every key I could a legal key to try to stop 
    the "forever" problem. 

    FYI, the CHOICE command is not generally available under NT. 
    You can get it on the disk version of the Resource Kit, but
    not on the download version. If you really want to implement
    this Win9x solution on NT, you can always steal a copy of 
    CHOICE.EXE from a Win9x box. All reports I've heard say it
    works just fine. If a Win98 box isn't handy, download it:
    ftp://ftp.microsoft.com/Services/TechNet/samples/PS/Win98/Reskit/SCRPTING/

    _______________________________________________________

    People running NT will be quickly frustrated by the 
    above examples.  QBASIC, however, runs on Win9x and 
    NT and offers a more universal solution. Try using the 
    QBASIC "sleep" command. In the batch file below, I 
    construct a simple two-line QBASIC program and run it. 
    Couldn't be easier. The problem is that pressing any key 
    immediately ends the time delay. At least there is no 
    "forever" problem.

@echo off
echo Starting!
echo sleep 10> sleep.bas
echo system>> sleep.bas
qbasic /run sleep.bas
echo Done!
del sleep.bas

    _______________________________________________________

    And here is the good QBASIC way, but this time using 
    the "on timer" function.

@echo off
echo Starting!
echo.on timer(10) gosub bail> sleep.bas
echo timer on>> sleep.bas
echo while -1>> sleep.bas
echo wend>> sleep.bas
echo system>> sleep.bas
echo bail:>> sleep.bas
echo system>> sleep.bas
qbasic /run sleep.bas
echo Done!
del sleep.bas

    The period after the echo command is what you have to 
    do to actually echo the word "on" in Win9x.

    _______________________________________________________

    One of the problems with the above methods is that 
    they all send CPU usage to 100%. Actually, this is 
    less of a problem than you might think if you are 
    working with short, tolerable time delays. If you 
    need a low a low-impact time delay, you're better 
    off switching to Windows Scripting. The 
    "Wscript.Sleep" command allows you to specify a 
    sleep time in milliseconds. So a time delay of 
    10 seconds would need a 10000 millisecond value. 
    Here is batch code for a ten-second delay that 
    creates the needed scripting file:

@echo off
echo Starting!
echo Wscript.Sleep 10000> sleep.vbs
start /w wscript.exe sleep.vbs
echo Done!
del sleep.vbs

    _______________________________________________________


Eric Phelps
http://www.ericphelps.com