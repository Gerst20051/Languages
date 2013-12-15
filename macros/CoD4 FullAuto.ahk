;iw3mp
num := 4
rate := 40
Numpad8::
num++
return
Numpad2::
num--
return
Numpad4::
rate -= 5
return
Numpad6::
rate +=5
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