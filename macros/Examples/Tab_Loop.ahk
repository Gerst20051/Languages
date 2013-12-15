Loop 20
{
    Send {Tab down}  ; Auto-repeat consists of consecutive down-events (with no up-events).
    Sleep 30  ; The number of milliseconds between keystrokes (or use SetKeyDelay).
}
Send {Tab up}  ; Release the key.