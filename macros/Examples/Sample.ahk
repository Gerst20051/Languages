;Run Website
#z::Run www.autohotkey.com

;Open Notepad
^!n::
IfWinExist Untitled - Notepad
	WinActivate
else
	Run Notepad
return