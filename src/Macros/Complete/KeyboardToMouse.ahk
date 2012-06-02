*#up::MouseMove, 0, -10, 0, R  ; Win+UpArrow hotkey => Move cursor upward
*#Down::MouseMove, 0, 10, 0, R  ; Win+DownArrow => Move cursor downward
*#Left::MouseMove, -10, 0, 0, R  ; Win+LeftArrow => Move cursor to the left
*#Right::MouseMove, 10, 0, 0, R  ; Win+RightArrow => Move cursor to the right

*<#RCtrl::  ; LeftWin + RightControl => Left-click (hold down Control/Shift to Control-Click or Shift-Click).
SendEvent {Blind}{LButton down}
KeyWait RCtrl  ; Prevents keyboard auto-repeat from repeating the mouse click.
SendEvent {Blind}{LButton up}
return

*<#AppsKey::  ; LeftWin + AppsKey => Right-click
SendEvent {Blind}{RButton down}
KeyWait AppsKey  ; Prevents keyboard auto-repeat from repeating the mouse click.
SendEvent {Blind}{RButton up}
return