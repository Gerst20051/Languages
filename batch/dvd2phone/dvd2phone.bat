@echo off
REM This batch file will take a DVD rip and recompress it to phone size.
REM The last line in this batch file does the actual conversion and is set to 
REM create an MP4 H264 at 15FPS and 100Kbps. This is fairly harsh compression, 
REM but it only takes about 120MB for a movie and looks great on a phone. 

REM REQUIRES "ffmpeg.exe" (and "pthreadGC2.dll") best obtained by installing
REM Videora iPod converter from: http://www.videora.com/ then copying files.
REM Ffmpeg needs H264 option compiled in, which is fairly rare. See also:
REM http://ffdshow.faireal.net/mirror/ffmpeg/
REM http://www.videohelp.com/tools/ffmpeg

REM REQUIRES the command-line version "MediaInfo"to get movie data. See:
REM http://mediainfo.sourceforge.net.
REM
REM MP4 files will play on Windows Mobile using TCPMP from:
REM http://picard.exceed.hu/tcpmp/test/
REM MP4 files will play on Windows. Try these players:
REM http://www.videolan.org/
REM http://mpui.sourceforge.net/

REM Because most phones (my phone) have a 4:3 screen ratio (1.33) but most DVD 
REM movie rips are in excess of 2:1, we need to do some cropping!
REM
REM If you insist on filling the screen, you'll have to throw away a huge amount
REM of the sides of the movie. If you insist on keeping everything, you'll end 
REM up with a skinny strip of movie you can't see with big black bars on top and 
REM bottom. My recommendation is to compromise and crop the movie to a 3:2 ratio
REM (1.50). This loses some of the sides, but it comes close to filling the screen.

REM Most phones will automatically fill in small black bars top & bottom to handle 
REM the discrepancy between the movie aspect ratio (1.50 if you take my advice) 
REM and the physical screen aspect ratio (1.33 if you have a 320x240 screen). 
REM In the event your phone gets confused, you can force black bars to be added 
REM to the actual movie. The "intermediate_ratio" below sets how much to crop the 
REM movie. Normally, you'll set the "final_ratio" to be the same number. If you 
REM want real black bars to be added to the movie, make the "final_ratio" match 
REM your phone's screen resolution. Obviously, the "final_width" should be your 
REM phone's screen width in pixels.

REM ########### USER SETTINGS ###########
REM To make the batch file integer math possible, 
REM all ratios are multipled by 100. For example, 
REM a 3:2 ratio of 1.50 is expressed here as 150. 
REM The "final_width" is the output size in pixels
set final_width=320
set intermediate_ratio=150
set final_ratio=133
REM #####################################

REM clear the variables if they were set by some other program
set original_height=
set original_width=
set pad_value=
set crop_value=
set intermediate_width=
set intermediate_height=
set final_height=

REM First check to see that needed programs actually exist!
cmd.exe /c ffmpeg.exe > %temp%\temp.txt
type %temp%\temp.txt | find /i "ffmpeg" > nul
if errorlevel 1 goto FFMPEG
cls
cmd.exe /c mediainfo.exe > %temp%\temp.txt
type %temp%\temp.txt | find /i "MediaInfo" > nul
if errorlevel 1 goto MEDIAINFO
cls
goto PROC

:FFMPEG
echo You need FFMPEG! The best way to get it is to install 
echo "Videora iPod Converter" from http://www.videora.com/ 
echo then copying the ffmpeg.exe and pthreadGC2.dll files 
echo into your "path" or in the same folder as this batch file.
pause
goto :EOF

:MEDIAINFO
echo You need MediaInfo! Please go to http://mediainfo.sourceforge.net
echo Get the command line version of the program and place it in your 
echo "path" or in the same folder as this batch file.
pause
goto :EOF

:PROC
REM find the original movie height and width
for /f %%x in ('mediainfo.exe --Inform^=Video^;%%Width%% %1') do set original_width=%%x
echo The movie original width is %original_width%
for /f %%x in ('mediainfo.exe --Inform^=Video^;%%Height%% %1') do set original_height=%%x
echo The movie original height is %original_height%

REM find the width needed to fit the intermediate ratio
set /a intermediate_width = original_height * intermediate_ratio / 100
echo For an intermediate ratio of %intermediate_ratio%/100, the width will be cropped to %intermediate_width%

REM calculate the left and right cropping
REM formula is (original_width - intermediate_width) / 2 
set /a crop_value = original_width - intermediate_width
set /a crop_value = crop_value / 2
echo We will crop %crop_value% from the left and right sides

REM we need the height for later
set /a intermediate_height = 100 * final_width / intermediate_ratio
set /a intermediate_height = (intermediate_height / 2) * 2
set /a final_height = 100 * final_width / final_ratio
set /a final_height = (final_height / 2) * 2

REM calculate the top and bottom padding
set /a pad_value = final_height - intermediate_height
set /a pad_value = pad_value / 2
set /a pad_value = (pad_value / 2) * 2
echo We will pad the top and bottom with black bands %pad_value% pixels high
echo This will boost the height from %intermediate_height% to %final_height%

REM force values to a multiple of 2
set /a crop_value = (crop_value / 2) * 2

REM now run the FFMPEG program to convert the movie
echo.
ffmpeg.exe -i %1 -cropleft %crop_value% -cropright %crop_value% -padtop %pad_value% -padbottom %pad_value% -aspect %final_ratio%:100 -s %final_width%x%intermediate_height% -loop 1 -f mp4 -r 15 -vcodec libx264 -level 13 -vb 100k -g 75 -coder 0 -threads auto -acodec libfaac -ac 1 -ab 16k -async 2 "%~d1%~p1%~n1.mp4"