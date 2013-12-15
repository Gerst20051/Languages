;iw3mp
num := 4
rate := 40
Numpad8::
num++
Send {o}%num%{enter}
return
Numpad2::
num--
Send {o}%num%{enter}
return
Numpad4::
rate -= 5
Send {o}%rate%{enter}
return
Numpad6::
rate += 5
Send {o}%rate%{enter}
return
~LButton::
state := GetKeyState("ScrollLock", "T")
If state {
	Loop %num%
	{
		Click
		Sleep %rate%
	}
}
return