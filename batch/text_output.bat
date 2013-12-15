batchtest.bat:
@echo foo
@echo bar
 
C:\>batchtest.bat
foo
bar

----------------------------------------------------

C:\> @echo foo&@echo bar
foo
bar

----------------------------------------------------

batchtest.bat:
@echo off
set foo=Line 1
echo y | set /p tmp="%foo%"
echo Line 2
echo Line 3
 
C:\>batchtest.bat
Line 1Line 2
Line 3
 
C:\>

----------------------------------------------------

C:\> echo y | set /p tmp="Line 1"> data.txt
C:\> echo y | set /p tmp="Line 2">> data.txt
C:\> echo y | set /p tmp="Line 3">> data.txt
C:\> type data.txt
Line 1Line 2Line 3