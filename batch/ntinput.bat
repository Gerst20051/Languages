   Getting user input with NT can be a problem 
   because most user input routines use the CON 
   and NUL device. NT, unfortunately, doesn't handle 
   these devices as files (like Win95 and DOS do). 
   Luckily, NT comes standard with QBASIC! Shown 
   here is a way to create a simple QBASIC program 
   that takes the user input and creates a temporary 
   batch file. When the batch file is run, an 
   environmental variable is set containing the 
   user input. 

---------------------------------------------------
@echo off
echo OPEN "~usrin.bat" FOR OUTPUT AS #1> ~usrin.bas
echo INPUT "Enter your name ", sUsrin$>> ~usrin.bas
echo PRINT #1, "set usrin="; sUsrin$>> ~usrin.bas
echo CLOSE #1>> ~usrin.bas
echo SYSTEM>> ~usrin.bas
qbasic /run ~usrin.bas
call ~usrin.bat
del ~usrin.bat
del ~usrin.bas
echo Your name is %usrin%
pause
cls
---------------------------------------------------

    I've had scattered reports that the above code 
    doesn't run on code page 437 (US) but works 
    on code page 850 (Multilingual). But I use 
    code page 437, and the code works identically 
    for me under Win95 and NT4. Just FYI.

    Eric Rose, someone who knows NT better than I 
    do, found that if you have problems with 
    QBASIC using expanded memory in ways NT 
    finds unacceptable, you can make everybody 
    happy by adding this line

---------------------------------------------------
set RTVMEXP=0
---------------------------------------------------

    to the beginning of the batch file (actually
    the second line, just after the "@echo off"
    line).

    In the above example, I create the QBASIC 
    code as it is needed. You can speed things up 
    considerably by creating the code ahead of time 
    and having it kept as a permanent item. Here is 
    the code rewritten as two separate files. 
    First, the batch file:

---------------------------------------------------
@echo off
qbasic /run userin.bas
call ~userin.bat
del ~userin.bat
echo Your name is %userin%
---------------------------------------------------

    Now the QBASIC file I call "USERIN.BAS"

---------------------------------------------------
OPEN "~userin.bat" FOR OUTPUT AS #1
INPUT "Enter your name ", sUsrin$
PRINT #1, "set userin="; sUsrin$
CLOSE #1
SYSTEM
---------------------------------------------------

    If you've kept your service packs up to date, 
    you got the Windows Scripting Host installed 
    when SP4 came out. If you have scripting, 
    this batch file should provide you with 
    another prettier alternative:

---------------------------------------------------
@echo off
 > ~userin.vbs echo strUserIn = InputBox("Enter Data")
>> ~userin.vbs echo Set fs = Wscript.CreateObject("Scripting.FileSystemObject")
>> ~userin.vbs echo strFileName = fs.BuildPath(Wscript.ScriptFullName ^& "\..", "~userin.bat")
>> ~userin.vbs echo strFileName = fs.GetAbsolutePathName(strFileName)
>> ~userin.vbs echo Set ts = fs.OpenTextFile(strFileName, 2, True)
>> ~userin.vbs echo ts.WriteLine "set userin=" ^& strUserIn
>> ~userin.vbs echo ts.Close
start /w wscript.exe ~userin.vbs
del ~userin.vbs
call ~userin.bat
del ~userin.bat
echo You entered %USERIN%
---------------------------------------------------

    Again, here is the above scripting version rewritten 
    as two separate files. First the batch file:

---------------------------------------------------
start /w wscript.exe userin.vbs
call ~userin.bat
del ~userin.bat
echo You entered %USERIN%
---------------------------------------------------

    Now the script file I call "userin.vbs"

---------------------------------------------------
strUserIn = InputBox("Enter Data")
Set fs = Wscript.CreateObject("Scripting.FileSystemObject")
strFileName = fs.BuildPath(Wscript.ScriptFullName & "\..", "~userin.bat")
strFileName = fs.GetAbsolutePathName(strFileName)
Set ts = fs.OpenTextFile(strFileName, 2, True)
ts.WriteLine "set userin=" & strUserIn
ts.Close
---------------------------------------------------

    While the "two separate files" version 
    above will run on any machine (so far) 
    that has scripting, the single file 
    version I showed only works on NT. 
    That's because the ampersand has special 
    meaning under NT. When the batch file 
    tries to "echo" a "&" character, NT 
    requires that a caret precede it:

---------------------------------------------------
>> ~userin.vbs echo ts.WriteLine "set userin=" ^& strUserIn
---------------------------------------------------

    The same line written for a Win9x machine would be:

---------------------------------------------------
>> ~userin.vbs echo ts.WriteLine "set userin=" & strUserIn
---------------------------------------------------

    If you have to write code that works 
    on both machines, you can test for the 
    behavior (which is safer than testing 
    for the OS). Here is code that will run 
    on NT and Win9x:

---------------------------------------------------
@echo off
:: This batch file illustrates how to create and run a script
:: file from a batch file. This uses scripting to solve the
:: common "How do I get user input" problem. This should work on
:: Win9x and NT boxes as long as they have scripting installed!

:: First test to see if we are on NT or similar OS
:: The ony difference is how they handle the ampersand
 > ~userin.vbs echo 1234&rem
type ~userin.vbs | find "rem" > nul
if errorlevel 1 goto WINNT
goto WIN9X

:WIN9X
 > ~userin.vbs echo strUserIn = InputBox("Enter Data")
>> ~userin.vbs echo Set fs = Wscript.CreateObject("Scripting.FileSystemObject")
>> ~userin.vbs echo strFileName = fs.BuildPath(Wscript.ScriptFullName & "\..", "~userin.bat")
>> ~userin.vbs echo strFileName = fs.GetAbsolutePathName(strFileName)
>> ~userin.vbs echo Set ts = fs.OpenTextFile(strFileName, 2, True)
>> ~userin.vbs echo ts.WriteLine "set userin=" & strUserIn
>> ~userin.vbs echo ts.Close
goto RUN

:WINNT
 > ~userin.vbs echo strUserIn = InputBox("Enter Data")
>> ~userin.vbs echo Set fs = Wscript.CreateObject("Scripting.FileSystemObject")
>> ~userin.vbs echo strFileName = fs.BuildPath(Wscript.ScriptFullName ^& "\..", "~userin.bat")
>> ~userin.vbs echo strFileName = fs.GetAbsolutePathName(strFileName)
>> ~userin.vbs echo Set ts = fs.OpenTextFile(strFileName, 2, True)
>> ~userin.vbs echo ts.WriteLine "set userin=" ^& strUserIn
>> ~userin.vbs echo ts.Close
goto RUN

:RUN
:: Now run the created script
start /w wscript.exe ~userin.vbs
del ~userin.vbs

:: Now call the created batch file
call ~userin.bat
del ~userin.bat

:: Now display the data!
echo You entered %USERIN%
pause
cls
---------------------------------------------------


    http://www.ericphelps.com