^!s::  ; Control+Alt+S hotkey.
IfWinNotExist Inbox - Microsoft Outlook
    return  ; Outlook isn't open to the right section, so do nothing.
WinActivate  ; Activate the window found by the above command.
Send ^n  ; Create new/blank e-mail via Control+N.
WinWaitActive Untitled Message
Send {Tab 2}Product Recall for ACME Rocket Skates  ; Set the subject line.
Send {Tab}Dear Sir or Madam,{Enter 2}We have recently discovered a minor defect ...  ; etc.
return  ; This line serves to finish the hotkey.