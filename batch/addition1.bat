    There are SEVERAL methods shown here. If you don't like
    one, just skip down to the next *** section.

    All the below code is for Win95/98. It will *probably* 
    also work on NT/XP/2000. FYI, 2000 and newer can do math
    with the SET command's /A option.

************************************************************
************************************************************
************************************************************


    Doing math in a batch file is a waste of time.
    There are no math functions. Luckily, all
    most people need to do is increment a counter
    of some sort. This can be done via string 
    comparisons -- something that batch files CAN 
    do. Here I show code which will increment a 
    three digit number each time it is called. 
    Looking at the code, you can see how easy it 
    would be to extend it to any length. The number 
    here is actually stored in the environment as 
    three separate variables E0, E1, and E2. To 
    refer to the entire number, you'd say this: 
    %E2%%E1%%E0%

------------------------------------------------------------
@echo off
:: Increments a three digit number
:: Works by comparing each digit
:: E2=hundreds, E1=tens, E0=ones
if [%E2%]==[] set E2=0
if [%E1%]==[] set E1=0
if [%E0%]==[] set E0=0
:E0
if %E0%==9 goto E1
if %E0%==8 set E0=9
if %E0%==7 set E0=8
if %E0%==6 set E0=7
if %E0%==5 set E0=6
if %E0%==4 set E0=5
if %E0%==3 set E0=4
if %E0%==2 set E0=3
if %E0%==1 set E0=2
if %E0%==0 set E0=1
goto DONE
:E1
set E0=0
if %E1%==9 goto E2
if %E1%==8 set E1=9
if %E1%==7 set E1=8
if %E1%==6 set E1=7
if %E1%==5 set E1=6
if %E1%==4 set E1=5
if %E1%==3 set E1=4
if %E1%==2 set E1=3
if %E1%==1 set E1=2
if %E1%==0 set E1=1
goto DONE
:E2
set E1=0
if %E2%==9 set E2=0
if %E2%==8 set E2=9
if %E2%==7 set E2=8
if %E2%==6 set E2=7
if %E2%==5 set E2=6
if %E2%==4 set E2=5
if %E2%==3 set E2=4
if %E2%==2 set E2=3
if %E2%==1 set E2=2
if %E2%==0 set E2=1
goto DONE
:DONE
------------------------------------------------------------

    The way the code is shown above, the environment 
    will "remember" the value of the counter. This 
    means the next time you start the program (if you 
    start it in the same memory space), it will 
    continue counting where it left off. This is 
    great sometimes, but a bother others. If you 
    want it to always start counting over, just add 
    code like this to the batch file just before the 
    line that calls the addition batch file:

set E2=0
set E1=0
set E0=0
call add.bat

    Or you can start the counting at some 
    predetermined number like 127:

set E2=1
set E1=2
set E0=7
call add.bat

    Of course, as soon as you call add.bat, the count 
    will be at 128.

************************************************************
************************************************************
************************************************************

    But there are times when you just don't 
    want the leading zeros that the above 
    method mandates. For that, real math is 
    handy. Here is a routine that will 
    increment any number. Because it uses 
    QBASIC, it works great only if you installed 
    QBASIC (generally true for NT, false for 9x).
------------------------------------------------------------
@echo off
if [%NUMBER%]==[] SET NUMBER=-1
> ~temp.bas echo PRINT "SET NUMBER="; LTRIM$(STR$(VAL(ENVIRON$("NUMBER")) + 1))
>>~temp.bas echo SYSTEM
qbasic /run ~temp.bas > ~temp.bat
call ~temp.bat
del ~temp.ba?
------------------------------------------------------------

    The above batch file needs some explaining. 
    A lot of explaining. First, if there is no 
    NUMBER environment variable, I create a 
    variable and set it's value to -1. Because 
    I plan on adding one to it, that means the 
    NUMBER variable will be zero when 
    everything is finished. Of course, if 
    there was already a NUMBER, it's left 
    alone -- for now! Next I create a two-line 
    QBASIC file named "~temp.bas" containing 
    this:

PRINT "SET NUMBER="; LTRIM$(STR$(VAL(ENVIRON$("NUMBER")) + 1))
SYSTEM

    The easy part is the second line SYSTEM. 
    All it does exit from QBASIC. The first 
    line is the magic. First I use QBASIC's 
    ENVIRON$ function to get the value of the 
    environment variable named NUMBER:
ENVIRON$("NUMBER")
    Now I use the VAL function to force what I 
    got to be considered as a number I can do 
    math with:
VAL(ENVIRON$("NUMBER"))
    Then I add one to to that number:
VAL(ENVIRON$("NUMBER")) + 1
    Now I want to convert the resulting number 
    back into a plain-text string by using the 
    STR$ function:
STR$(VAL(ENVIRON$("NUMBER")) + 1)
    And finally, I use the LTRIM$ function to 
    remove any leading spaces (because QBASIC 
    always reserves a space there for a minus 
    sign):
LTRIM$(STR$(VAL(ENVIRON$("NUMBER")) + 1))
    Then I just use the PRINT method to put the 
    text "SET NUMBER=" in front of that number. 
    Well... PRINT just dumps stuff to the 
    screen, or "Standard Out". But we don't want 
    the number on the screen, we want it in a 
    file! Luckily, we can rely on the DOS batch 
    file's redirection ability (the 
    "greater-than" symbol) to handle this:
qbasic /run ~temp.bas > ~temp.bat
    When QBASIC is run, the redirection insures 
    that all screen output is sent to the 
    "~temp.bat" file. That means we don't have 
    to rely on QBASIC to open and write to a 
    file. We just do a generic PRINT and let 
    DOS redirection handle the file creation.

    After QBASIC runs, a "~temp.bat" will exist 
    with a line like "SET NUMBER=0" in it. So 
    we call that batch file, then delete all the 
    temporary created files (both ~remp.bas and 
    ~temp.bat) with the single "del ~temp.ba?" 
    line. 

    If you are worried that someone won't have 
    QBASIC on their system, you can either call 
    this batch file here:

http://www.ericphelps.com/batch/samples/getqbasic.bat.txt

    Or just add your code to the end of it.

************************************************************
************************************************************
************************************************************

    Batch does have the ability to count. Sort 
    of. The FIND command can count lines in a 
    file that contain certain strings. So here 
    I show a way to use a file "~counter.txt" 
    that gets a new "X" added to it every time. 
    I can count the lines to retrieve the value. 
    The cool thing about this method is that 
    several different programs in different 
    memory space can be using the same counter 
    file. What's more, because the counter value 
    is maintained in a file, the count is preserved 
    even through reboots! If you just wanted to read 
    the value without incrementing it, just leave 
    off the second line (echo X >> ~counter.txt). 
    If you want to set the counter file to a value 
    of zero in your code, use a line like this:
echo.> ~counter.txt
    and if you want it set to a value of one:
echo X > ~counter.txt

------------------------------------------------------------
@echo off
echo X >> ~counter.txt
type ~counter.txt | find /c "X" > ~temp.txt
fc ~temp.txt nul /lb1 /n | date | find "1:" > en#er.bat
del ~temp.txt > nul
echo set number=%%5> enter.bat
call en#er.bat
del en?er.bat > nul
------------------------------------------------------------

************************************************************
************************************************************
************************************************************

    All the above methods are standalone counters. They 
    are generally called by other batch files whenever 
    an incremented number is needed. Here is the opposite 
    -- a counter batch file that calls a separate process 
    batch file. The counter here is implemented as a 
    series of loops. It will count from 000 to 999, 
    calling a "process.bat" that we'll assume will have 
    some use for the number!

------------------------------------------------------------
@echo off
if [%1]==[] goto NONE
if [%2]==[] goto ONE
if [%3]==[] goto TWO
goto THREE

:NONE
for %%x in (0 1 2 3 4 5 6 7 8 9) do call %0 %%x
goto DONE

:ONE
for %%x in (0 1 2 3 4 5 6 7 8 9) do call %0 %1 %%x
goto DONE

:TWO
for %%x in (0 1 2 3 4 5 6 7 8 9) do call %0 %1 %2 %%x
goto DONE

:THREE
call PROCESS.BAT %1%2%3
goto DONE

:DONE
------------------------------------------------------------
