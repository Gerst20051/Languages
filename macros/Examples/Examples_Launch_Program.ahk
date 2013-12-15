#z::
Run, Notepad, , max
Run, Notepad, , min
return

#c::Run, %comspec% /k, C:\My Documents

Run, %comspec% /c dir >"Output File.txt", C:\My Documents
Run, Notepad.exe "C:\My Documents\Address List.txt"
Run, "%A_AhkPath%" "C:\Scripts\Test Script.ahk" param1 "param2 with spaces" param3

Run, properties "C:\Address List.txt"
Run, print "C:\Address List.txt"

RunWait, %comspec% /c dir c:\NonExistent.txt, , hide
MsgBox, %ErrorLevel%