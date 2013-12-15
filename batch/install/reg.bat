xcopy .\install\boot.ini c: /y /h
net localgroup "administrators" "asbury" /add
net localgroup "administrators" "bgesstudent" /add
net localgroup "administrators" "csesstudent" /add
net localgroup "administrators" "gemstudent" /add
net localgroup "administrators" "elhsstudent" /add
net localgroup "administrators" "elmsstudent" /add
net localgroup "administrators" "isesstudent" /add
net localgroup "administrators" "lcststudent" /add
net localgroup "administrators" "lhsstudent" /add
net localgroup "administrators" "lmsstudent" /add
net localgroup "administrators" "lmestudent" /add
net localgroup "administrators" "nbestudent" /add
net localgroup "administrators" "nlhsstudent" /add
net localgroup "administrators" "pcesstudent" /add
net localgroup "administrators" "pcmsstudent" /add
net localgroup "administrators" "rsestudent" /add
net localgroup "administrators" "srlestudent" /add
net localgroup "administrators" "student" /add
net localgroup "administrators" "sjesstudent" /add
net localgroup "administrators" "uestudent" /add
net localgroup "administrators" "wlhsstudent" /add
net localgroup "administrators" "wlmsstudent" /add

ECHO.
ECHO Applying Registry Tweaks...
.\install\RegTweaks.reg
start /wait %systemdrive%\install\regtweaks.reg

start /wait d:\applications\shockwave10.exe
start /wait d:\applications\java.exe
start /wait d:\applications\norton10\setup.exe
start /wait d:\applications\ie7\IE7Setup.exe
Start /wait D:\ignorethemessageandclickok