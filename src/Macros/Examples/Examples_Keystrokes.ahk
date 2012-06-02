Send {DEL 4}  ; Presses the Delete key 4 times.
Send {S 30}   ; Sends 30 uppercase S characters.
Send +{TAB 4}  ; Presses Shift-Tab 4 times.
Send {b down}{b up}
Send {TAB down}{TAB up}
Send {Up down}  ; Press down the up-arrow key.
Sleep 1000  ; Keep it down for one second.
Send {Up up}  ; Release the up-arrow key.
Send Sincerely,{enter}John Smith  ; Types a two-line signature.
Send !fs ; Select the File->Save menu (Alt+F followed by S).
Send {End}+{Left 4} ; Jump to the end of the text then send four shift+left-arrow keystrokes.
SendInput {Raw}A long series of raw characters sent via the fastest method (SendInput).