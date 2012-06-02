; Basic examples:
GetKeyState, state, RButton  ; Right mouse button.
GetKeyState, state, Joy2  ; The second button of the first joystick.

GetKeyState, state, Shift
if state = D
    MsgBox At least one Shift key is down.
else
    MsgBox Neither Shift key is down.

GetKeyState, state, CapsLock, T ;  D if CapsLock is ON or U otherwise.
state := GetKeyState("Capslock", "T")  ; True if CapsLock is ON, false otherwise.

; Remapping example (this is only for illustration because it would be easier to use
; the built-in remapping feature):
; In the following hotkey, the mouse button is kept held down while NumpadAdd is
; down, which effectively transforms NumpadAdd into a mouse button. This method
; can also be used to repeat an action while the user is holding down a key or button:
*NumpadAdd::
MouseClick, left,,, 1, 0, D  ; Hold down the left mouse button.
Loop
{
    Sleep, 10
    GetKeyState, state, NumpadAdd, P
    if state = U  ; The key has been released, so break out of the loop.
        break
    ; ... insert here any other actions you want repeated.
}
MouseClick, left,,, 1, 0, U  ; Release the mouse button.
return

; Example: Make joystick button behavior depend on joystick axis position.
joy2:: 
GetKeyState, joyx, JoyX 
if joyx > 75 
    MsgBox Action #1 (button pressed while joystick was pushed to the right).
else if joyx < 25 
    MsgBox Action #2 (button pressed while joystick was pushed to the left).
else
    MsgBox Action #3 (button pressed while joystick was centered horizontally).
return

; Hold Breath / Aim CoD
shift = 0
~RButton::
if shift = 0
{ 
Send, {ShiftDown}
shift = 1
}
else 
{
Send, {ShiftUp}
Shift = 0
}
Return,