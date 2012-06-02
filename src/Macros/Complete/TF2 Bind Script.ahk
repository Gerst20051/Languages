/*
 AutoHotkey Version: 1.x
 Language:       English
 Platform:       Win9x/NT
 Author:         Die_Angel <dieangel@ethernia.hd.free.fr>

 Script Function:
	TF2 bind script.
*/
#SingleInstance ignore
#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

Numpad_menu_open 	= 0

disguise_menu_open 	= 0
talk_menu_open 		= 0

Current_class 		= 1
Current_team 		= 0 ; 0 = blu 1 = red
script_override		= 0
;1scout 2soldier 3pyro 4demo 5heavy 6tech 7medic 8sniper 9snpy
active = false
Title = ""

;;;class specific values to retain
curr_tool = 0
; 0=needlegun 	1=melee 		2= healgun
; 4=scattergun	5= pistol (scout & engies)
; 6=rocket 		7=shotgun		
; 9=fireaxe		10=flamethrower
; 11=grenade launcher
; 13=sticky
; 14=minigun
; 15 dispenser 16 sentry 17 tp in 18 tp out
; 19 machinegun			20 sniper rifle
; 21 colt 22 sapper


NumpadAdd::			team_select( true )
NumpadAdd up::		team_select( false )

NumpadIns::			disguise_team( true )
NumpadIns up::		disguise_team( false )
Numpad0::			disguise_team( true )
Numpad0 up::		disguise_team( false )

NumpadDel::			spy_talk( true)
NumpadDel up::		spy_talk( false )
NumpadDot::			spy_talk( true)
NumpadDot up::		spy_talk( false )

NumpadEnter::		class_menu( true )
NumpadEnter up::	class_menu( false )

NumpadEnd:: 	class_selection(1, true  , "NumpadEnd" )
NumpadEnd up::	class_selection(1, false , "NumpadEnd" )
Numpad1:: 		class_selection(1, true  , "NumpadEnd" )
Numpad1 up:: 	class_selection(1, false  , "NumpadEnd" )
NumpadDown:: 	class_selection(2, true  , "NumpadDown" )
NumpadDown up:: class_selection(2, false , "NumpadDown" )
Numpad2:: 		class_selection(2, true  , "NumpadDown" )
Numpad2 up:: 	class_selection(2, false  , "NumpadDown" )
NumpadPgDn:: 	class_selection(3, true  , "Numpad3" )
NumpadPgDn up::	class_selection(3, false , "Numpad3" )
Numpad3:: 		class_selection(3, true  , "Numpad3" )
Numpad3 up:: 	class_selection(3, false  , "Numpad3" )
NumpadLeft::	class_selection(4, true  , "Numpad4" )
NumpadLeft up:: class_selection(4, false , "Numpad4" )
Numpad4:: 		class_selection(4, true  , "Numpad4" )
Numpad4 up:: 	class_selection(4, false  , "Numpad4" )
NumpadClear::	 class_selection(5, true  , "Numpad5" )
NumpadClear up:: class_selection(5, false , "Numpad5" )
Numpad5:: 		class_selection(5, true  , "Numpad5" )
Numpad5 up:: 	class_selection(5, false  , "Numpad5" )
NumpadRight::	 class_selection(6, true  , "Numpad6" )
NumpadRight up:: class_selection(6, false , "Numpad6" )
Numpad6:: 		class_selection(6, true  , "Numpad6" )
Numpad6 up:: 	class_selection(6, false  , "Numpad6" )
NumpadHome::	 class_selection(7, true  , "Numpad7" )
NumpadHome up::	 class_selection(7, false , "Numpad7" )
Numpad7:: 		class_selection(7, true  , "Numpad7" )
Numpad7 up:: 	class_selection(7, false  , "Numpad7" )
NumpadUp::		 class_selection(8, true  , "Numpad8" )
NumpadUp up::	 class_selection(8, false , "Numpad8" )
Numpad8:: 		class_selection(8, true  , "Numpad8" )
Numpad8 up:: 	class_selection(8, false  , "Numpad8" )
NumpadPgUp::	 class_selection(9, true  , "Numpad9" )
NumpadPgUp up::	 class_selection(9, false , "Numpad9" )
Numpad9:: 		class_selection(9, true  , "Numpad9" )
Numpad9 up:: 	class_selection(9, false  , "Numpad9" )
F10::
	if( Is_TF2() )
	{
		if( curr_tool = 2 )
			Send {Blind}{F11}
		else
			Send {Blind}{F12}
	}
return
Enter::
	Tooltip
	script_override = 0
	Send {Blind}{Enter Down}
return
Enter up::
	if( NOT Is_TF2() )
	Send {Blind}{Enter Up}
return

SC033::
	if( Is_TF2() )
	{
		Tooltip,SCRIPT OVERRIDE ON
		script_override = 1
	}
	Send {Blind}{SC033 Down}
return
SC033 up::
	Send {Blind}{SC033 Up}
return
SC032::
	if( Is_TF2() )
	{
		Tooltip,SCRIPT OVERRIDE ON
		script_override = 1
	}
	Send {Blind}{SC032 Down}
return
SC032 up::
	Send {Blind}{SC032 Up}
return


class_selection(class_num,keydown,keycode) ;scout... class_selection(1,keydown,NumpadEnd)
{
	
	global Numpad_menu_open
	if ( Is_TF2() AND ( Numpad_menu_open AND keydown ) )
		SetClass(class_num)
	else
		Send,{Blind}{ %keycode% } ;note, it seems binds do not like keys that are holding down up
}
team_select( keydown )
{
	if ( Is_TF2() )
	{
		if( keydown AND Is_Spy() )
		{
			global Current_team
			msg = ""
			if (Current_team)
			{
				msg = Team set as BLU
				load_disguise_red()
				Current_team = 0
			}
			else
			{
				msg = Team set as RED
				load_disguise_blu()
				Current_team = 1
			}
			ToolTip,%msg%
			SetTimer, mess_fade, 5000 ;extra thread fork to fade the message
		}
	}
	else
	{
		if( keydown )
			Send {Blind}{NumpadAdd Down}
		else
			Send {Blind}{NumpadAdd up}
	}
}

disguise_team( keydown )
{
	if ( Is_TF2() AND Is_Spy() )
	{
		global Current_team
		global disguise_menu_open
		if ( keydown )
		{
			if ( disguise_menu_open = 0 )
			{
				disguise_menu_open = 1
				Tooltip,Disguising as your own team
				if ( Current_team )
					load_disguise_red()
				else
					load_disguise_blu()
			}
		}
		else
		{
			if ( disguise_menu_open )
			{
				disguise_menu_open = 0
				Tooltip
				if ( Current_team )
					load_disguise_blu()
				else
					load_disguise_red()
			}
		}
	}
	else
	{
		if ( keydown )
			Send {Blind}{NumpadIns Down}
		else
			Send {Blind}{NumpadIns up}
	}
}

spy_talk( keydown)
{
	if ( Is_TF2() AND Is_Spy())
	{
		global talk_menu_open
		global Current_team
		if ( keydown )
		{
			if( talk_menu_open = 0 )
			{
				talk_menu_open = 1
				Tooltip,Talk mode
				load_voice()
			}
		}
		else
		{
			if( talk_menu_open )
			{
				talk_menu_open = 0
				if ( Current_team )
					load_disguise_blu()
				else
					load_disguise_red()
				Tooltip
			}
		}
	}
	else
	{
		if ( keydown )
			Send {Blind}{NumpadDel Down}
		else
			Send {Blind}{NumpadDel up}
	}
}

mess_fade:
	SetTimer, mess_fade, Off
	ToolTip
return

class_menu( keydown )
{
	global Numpad_menu_open
	if ( Is_TF2() )
	{
		if ( keydown )
		{
			Numpad_menu_open = 1
			ToolTip,
			(
	Custom class loading menu
			
	1. Scout
	2.Soldier
	3.Pyro
	4.Demoman
	5.Heavy
	6.Engineer
	7.Medic
	8.Sniper
	9.Spy
	
			)
		}
		else
		{
			Numpad_menu_open = 0
			ToolTip
		}
	}
	else
	{
		if ( keydown )
			Send {Blind}{NumpadEnter Down}
		else
			Send,{Blind}{NumpadEnter Up}
	}
}

;;;;;;;;;;;;;;;;;;;mouse buttons
WheelDown::
	if ( Is_TF2() )
		if ( Is_Tech() )
		{
			if ( curr_tool = 1)
				build_dispenser()
			else if ( curr_tool = 15)
				build_sentry()
			else if ( curr_tool = 16)
				build_tp_in()
			else if ( curr_tool = 17)
				build_tp_out()
			else
				switch_melee()	
		}
		else if ( Is_Spy() )
		{
			if ( curr_tool = 1)
				switch_colt()
			else
				switch_melee()
		}
		else
			switch_melee()	
	else
		Send,{Blind}{WheelDown}
return

WheelUp::
	if Is_TF2()
	{
		if ( Is_Medic() )
			switch_needlegun()
		else if ( Is_Scout() OR Is_Tech() )
			switch_pistol()
		else if ( Is_Soldier() OR Is_Pyro() OR Is_Heavy() )
			switch_shotgun()
		else if ( Is_Demo() )
		{
			if ( curr_tool = 11)
				switch_sticky()
			else
				switch_grenadelauncher()
		}
		else if( Is_Sniper() )
			switch_machinegun()
		else if( Is_Spy() )
			switch_sapper()
	}
	else
		Send,{Blind}{WheelUp}
return

RButton::
	if Is_TF2()
	{
		if ( Is_Medic() )
		{
			if ( curr_tool = 2)
				attack2()
			else
				switch_medigun()
		}
		else if ( Is_Scout() )
		{
			if ( curr_tool = 4 )
				switch_melee()	
			else
				switch_scattergun()
		}
		else if ( Is_Soldier() )
if ( curr_tool = 6)
				switch_shotgun()
			else
				switch_rocketlauncher()
		else if ( Is_Pyro() )
if ( curr_tool = 10 )
				switch_shotgun()
			else
				switch_flamethrower()
		else if ( Is_Demo() OR Is_Spy() )
			attack2()
		else if ( Is_Heavy() )
		{
			if ( curr_tool = 14 )
				Send,{Blind}{RButton down}
			else
				switch_minigun()
		}
		else if ( Is_Tech() )
		{
			if (curr_tool = 7)
				switch_melee()
			else
				switch_shotgun()
		}
		else if ( Is_Sniper() )
		{
			if ( curr_tool = 20 )
				attack2()
			else
				switch_sniper()
		}
	}
	else
		Send,{Blind}{RButton down}
return
RButton up::
	if Is_TF2()
	{
		if ( Is_Heavy() AND curr_tool = 14)
			Send,{Blind}{RButton up}
	}
	else
		Send,{Blind}{RButton up}
return
LButton::
	if Is_TF2()
	{
		if ( curr_tool = 2 )
			attack_off()
		else if ( curr_tool = 13 )
		{
			SetTimer, StickyReload, Off
			attack_on()
		}
		else
			attack_on()
	}
	else
		Send,{Blind}{LButton down}
return
LButton up::
	if Is_TF2()
	{
		if ( curr_tool = 2 ) ;medigun
			attack_on()
		else if ( curr_tool = 0 OR curr_tool = 19 ) ;needlegun or machinegun
		{
			attack_off()
			reload()
		}
		else if ( curr_tool = 13) ;stickygun
		{
			attack_off()
			reloader_sticky()
		}
		else if ( curr_tool = 11 OR curr_tool = 4 OR curr_tool = 6 OR curr_tool = 7  )
		{
			attack_off()
			reload2()
		}
		else if ( Is_Spy() AND curr_tool = 1)
		{
			attack_off()
			last_disguise()
		}
		else
			attack_off()
	}
	else
		Send,{Blind}{LButton up}
return

;sticky reloader execute only once
StickyReload:
	SetTimer, StickyReload, Off
	reload()
return

Is_TF2()
{
	WinGetActiveTitle, A
	global Title = A
	if ( global Title = "Team Fortress 2")
	{
		if ( script_override = 1 )
			return false
		else
			return true
	}
	else
		return false
}
Is_Medic()
{
	global Current_class
	if ( Current_class = 7 )
		return true
	else
		return false
}
Is_Scout()
{
	global Current_class
	if ( Current_class = 1 )
		return true
	else
		return false
}
Is_Soldier()
{
	global Current_class
	if ( Current_class = 2 )
		return true
	else
		return false
}
Is_Pyro()
{
	global Current_class
	if ( Current_class = 3 )
		return true
	else
		return false
}
Is_Demo()
{
	global Current_class
	if ( Current_class = 4 )
		return true
	else
		return false
}
Is_Heavy()
{
	global Current_class
	if ( Current_class = 5 )
		return true
	else
		return false
}
Is_Tech()
{
	global Current_class
	if ( Current_class = 6 )
		return true
	else
		return false
}
Is_Sniper()
{
	global Current_class
	if ( Current_class = 8 )
		return true
	else
		return false
}
Is_Spy()
{
	global Current_class
	if ( Current_class = 9 )
		return true
	else
		return false
}
SetClass(chosen_class)
{
	global Current_class := chosen_class
	global curr_tool
	dclass = "nil"
	load_voice()
	
	if ( Is_Scout() )
	{
		dclass = "Scout"
		curr_tool = 4
	}
	else if ( Is_Soldier() )
	{
		dclass = "Soldier"
		curr_tool = 6
	}
	else if ( Is_Pyro() )
	{
		dclass = "Pyro"
		curr_tool = 10
	}
	else if ( Is_Demo() )
	{
		dclass = "Demoman"
		curr_tool = 11
	}
	else if ( Is_Heavy() )
	{
		dclass = "Heavy"
		curr_tool = 14
	}
	else if ( Is_Tech() )
	{
		dclass = "Engineer"
		curr_tool = 7
	}
	else if ( Is_Medic() )
	{
		dclass = "Medic"
		curr_tool = 0
	}
	else if ( Is_Sniper() )
	{
		dclass = "Sniper"
		curr_tool = 20
	}
	else if ( Is_Spy() )
	{
		dclass = "Spy"
		curr_tool = 21
	}
	Send,{Blind}{SC034}{Shift down}
	Sleep,50
	Send,{Blind}{%Current_class%}
	Sleep,50
	Send,{Blind}{Shift up}
	Sleep,50
	Send,{Blind}{M}
	ToolTip,% dclass . " config selected."
}

;;;;;;;;;;;;;;keys
attack_on()
{
	Send,{Blind}{LButton down}
}
attack_off()
{
	Send,{Blind}{LButton up}
}
attack2()
{
	Send,{Blind}{RButton}
}
reload()
{
	reload2()
	reload2_off()
}
reload2()
{
	Send,{Blind}{XButton1 down}
}
reload2_off()
{
	Send,{Blind}{XButton1 up}
}
reloader_sticky()
{
	SetTimer, StickyReload, 1000 ;extra thread fork to reload stickies
}

;weapon switches
gun_reset()
{
	SetTimer, StickyReload, Off
	reload2_off()
	attack_off()
}
switch_needlegun()
{
	gun_reset()
	global curr_tool = 0
	Send,{Blind}{1}
}
switch_melee()
{
	gun_reset()
	global curr_tool = 1
	Send,{Blind}{3}
}
switch_medigun()
{
	gun_reset()
	global curr_tool = 2
	Send,{Blind}{2}
}
switch_scattergun() ;NOTE: scattergun has a certain.. delay before triggering it's reload, so it is reloading permanently.
{
	attack_off()
	global curr_tool = 4
	Send,{Blind}{1}
	reload2()
}
switch_pistol()
{
	gun_reset()
	global curr_tool = 5
	Send,{Blind}{2}
}
switch_rocketlauncher() ;i think the rocketlauncher behave as the scatter gun/shotgun so ...
{
	attack_off()
	global curr_tool = 6
	Send,{Blind}{1}
	reload2()
}
switch_shotgun() ;meh ...
{
	attack_off()
	global curr_tool = 7
	if ( Is_Tech() )
		Send,{Blind}{1}
	else
		Send,{Blind}{2}
	reload2()
}
switch_flamethrower()
{
	gun_reset()
	global curr_tool = 10
	Send,{Blind}{1}
}
switch_grenadelauncher() ;looks like the grenade launcher behave as the scatter gun/shotgun too ...
{
	attack_off()
	SetTimer, StickyReload, Off
	global curr_tool = 11
	Send,{Blind}{1}
	reload2()
}
switch_sticky()
{
	gun_reset()
	global curr_tool = 13
	Send,{Blind}{2}
	reloader_sticky()
}
switch_minigun()
{
	gun_reset()
	global curr_tool = 14
	Send,{Blind}{1}
}

;UIOP
build_dispenser()
{
	gun_reset()
	global curr_tool = 15
	Send,{Blind}{U}
}
build_sentry()
{
	gun_reset()
	global curr_tool = 16
	Send,{Blind}{I}
}
build_tp_in()
{
	gun_reset()
	global curr_tool = 17
	Send,{Blind}{O}
}
build_tp_out()
{
	gun_reset()
	global curr_tool = 18
	Send,{Blind}{P}
}

;sniper
switch_machinegun()
{
	gun_reset()
	global curr_tool = 19
	Send,{Blind}{2}
}
switch_sniper()
{
	gun_reset()
	global curr_tool = 20
	Send,{Blind}{1}
}
switch_colt()
{
	gun_reset()
	global curr_tool = 21
	Send,{Blind}{1}
}
switch_sapper()
{
	gun_reset()
	global curr_tool = 22
	Send,{Blind}{2}
}
load_voice()
{
	Send,{Blind}{M}
}
last_disguise()
{
	Send,{Blind}{J}
}
load_disguise_red()
{
	Send,{Blind}{L}
}
load_disguise_blu()
{
	Send,{Blind}{K}
}