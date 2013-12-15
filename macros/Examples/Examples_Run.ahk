Run Notepad
Run C:\My Documents\Address List.doc
Run C:\My Documents\My Shortcut.lnk
Run www.yahoo.com
Run mailto:someone@somedomain.com

#n::Run Notepad
^!c::Run calc.exe

#n::
Run http://www.google.com
Run Notepad.exe
return

Run %A_ProgramFiles%\Winamp\Winamp.exe

RunWait Notepad
MsgBox The user has finished (Notepad has been closed).