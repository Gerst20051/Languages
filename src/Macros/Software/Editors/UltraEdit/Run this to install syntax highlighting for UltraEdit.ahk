;##############################################################################
;#
;#  Add or update syntax highlighting for AutoHotKey scripts in UltraEdit
;#
;#  Mod of a script done by Tekl (although not much has survived)
;#  Mod done by toralf, 2005-11-14
;#              tonne, 2009-04-21 (UltraEdit 15)
;#
;#  Tested with: AHK 1.0.40.06, UltraEdit 11.10a, UltraEdit 15.01
;#
;#  Requirements
;#    - Syntax files for AHK in one directory
;#    - UltraEdit uses standard file for highlighting => wordfile.txt
;#
;#  Customize:
;#    - The default color for strings is gray, change it to any color
;#         you want to have "string" to appeer => Extra->Option->syntaxhiglighting
;#    - Change the default color for up to 8 keyword groups
;#         => Extra->Option->syntaxhiglighting
;#    -specify up to 8 syntax files, each containing one keyword per line
;#         => you can add your own files, for keywords that you want to highlight
;#            Personally I use 3 additional: Operators, Separators and Special
;#            The content of these files is posted further down
;#

; Specify a list of up to 8 syntax files; the order influences the color given to them by UE by default
SyntaxFileNameList = CommandNames|Keywords|Variables|Functions|Keys|Operators|Separators|Special
;Default colors in UE:  blue     |red     |orange   |green    |brown|blue    |blue      |blue

SyntaxExtention = .txt

SetWorkingDir %A_ScriptDir%  ; Ensure consistent starting directory so that any UE-specific syntax files can found.
GoSub, LoadInformation
GoSub, ShowInformation

return

ShowInformation:
  Gui,Add,Text,x005                       , Installation of syntaxfiles for UltraEdit
  Gui,Add,Text,x005 yp+24                 , Version
  Gui,Add,Edit,x085 yp-03 vUEVersion      , %UEVersion%
  Gui,Add,Text,x005 yp+27                 , Use registry
  Gui,Add,Text,x085 yp+00                 , % UERegistry ? "Yes" : "No"
  Gui,Add,Text,x005 yp+24                 , UltraEdit path
  Gui,Add,Edit,x085 yp-03 vUEpath         , %UEPath%
  Gui,Add,Text,x005 yp+27                 , Ini file location
  Gui,Add,Edit,x085 yp-03 vUEini          , %UEini%
  Gui,Add,Text,x005 yp+27                 , Wordfile
  Gui,Add,Edit,x085 yp-03 vUEWordFile     , %UEWordFile%
  Gui,Add,Text,x005 yp+27                 , Languages
  Gui,Add,Edit,x085 yp-03 vUELanguages    , %UELanguages%
  Gui,Add,Text,x115 yp+03                 , (AHK is #%UELanguage%)
  Gui,Add,Text,x005 yp+24                 , AHK syntax files
  Gui,Add,Edit,x085 yp-03 vAHKSyntax      , %AHKSyntax%
  Gui,Add,Button,x5 yp+51 default gInstall, Install syntax file
  Gui,Show
return



; ------------------------------------------------------------------------------------------------------------
; LOAD INFORMATION
LoadInformation:
  GoSub, GetPath
  GoSub, GetIniFile
  GoSub, GetConfig
  GoSub, GetWordFile
  GoSub, GetSyntaxFiles
return

GetPath:
  RegRead, UEPath, HKEY_LOCAL_MACHINE, SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\UEDIT32.exe,Path
  IfNotExist, %UEPath%\uedit32.exe
  {
    UEPath = %A_ProgramFiles%\UltraEdit
  }
return

GetConfig:
  IniRead, UERegistry, %UEini%, Settings, Use Registry, "0"
  IniRead, UEversion, %UEini%, Settings, Version
  UEVersion := RegExReplace(UEVersion,"[^0-9.]")
return

GetIniFile:
  UEini = %APPDATA%\IDMComp\UltraEdit\uedit32.ini
  IfNotExist, %UEini%
  {
    UEini = %A_WinDir%\uedit32.ini
  }
Return

GetWordFile:
  If (UEVersion >= 15.00)
  {
  	UEWordPath = %A_AppData%\IDMComp\UltraEdit\wordfiles
    UEWordFile = %UEWordPath%\AutoHotkey.uew
	  UELanguages = 0
	  Loop, %UEWordPath%\*.uew
	  {
	  	Loop, Read, %A_LoopFileLongPath%
	  	{
	    	StringLeft, WFdef, A_LoopReadLine, 2
	    	If WFdef = /L
	    	{
	      	StringSplit, WFname, A_LoopReadLine, " ;"
	      	LanguageName = %WFname2%
	      	Language := SubStr(WFName1,3)
	      	If LanguageName = AutoHotkey
	      		UELanguage   = %Language%
        	If (Language > UELanguages)
        		UELanguages = %Language%
	    	}
	  	}
	 	}
	 	If Not UELanguage 
	 		UELanguage := UELanguages+1
  }
	Else
	{
		IniRead, UEWordFile, %UEini%, Settings, Language File
  		
	  ;Check the number of languages in the current wordfile
	  UELanguages = 0
	  Loop, Read, %UEWordFile%
	  {
	    StringLeft, WFdef, A_LoopReadLine, 2
	    If WFdef = /L
	    {
	      StringSplit, WFname, A_LoopReadLine, "      ;"
	      LanguageName = %WFname2%
	      If LanguageName <> AutoHotkey
	        UELanguages += 1
	    }
	  }
	}
Return

GetSyntaxFiles:
  ; Discover where AutoHotkey and its related files reside:
  RegRead, ahkpath, HKLM, SOFTWARE\AutoHotkey, InstallDir
  if (ErrorLevel or not FileExist(ahkpath . "\AutoHotkey.exe"))  ; Not found, so try best-guess instead.
	  SplitPath, A_AhkPath,, ahkpath

  AHKSyntax = %ahkpath%\Extras\Editors\Syntax
  IfNotExist, %AHKSyntax%
  {
    AHKSyntax = %A_ProgramFiles%\AutoHotkey\Extras\Editors\Syntax
  }
return

; ------------------------------------------------------------------------------------------------------------
; INSTALL SYNTAX FILES
Install:
  GoSub, Validate
  GoSub, doInstall
return

Validate:
  Gui,Submit
  If Not UEPath
  {
    MsgBox, 16,, Ultra Edit not found
    ExitApp
  }
  If Not UEIni
  {
    MsgBox, 16,, Ultra Edit ini file not found
    ExitApp
  }
  If Not UEWordFile
  {
    MsgBox, 16,, Ultra Edit word file not found
    ExitApp
  }
  If Not AHKSyntax
  {
    MsgBox, 16,, Syntax files not found
    ExitApp
  }
  GoSub, ValidateSyntax
return

ValidateSyntax:
  MissingFile =
  FileCount = 0
  Loop, Parse, SyntaxFileNameList, |
  {
    FileCount += 1
    IfNotExist, %AHKSyntax%\%A_LoopField%%SyntaxExtention%
    {
      IfNotExist, %A_LoopField%%SyntaxExtention%     ; allow files to reside locally
        MissingFile = %MissingFile%`n%A_LoopField%%SyntaxExtention%
    }
  }
  If MissingFile is not Space
  {
    MsgBox, 16,, AHK Syntax file(s)`n%MissingFile%`n`ncannot be found in`n`n%AHKSyntax%\.
    ExitApp
  }
  If FileCount > 8
  {
    MsgBox, 16,, You have specified %FileCount% Syntax files.`nOnly 8 are supported be UltraEdit.`nPlease shorten the list.
    ExitApp
  }
return

doInstall:

  ;#############   Read keywords from syntax files into arrays   ################
  Loop, Parse, SyntaxFileNameList, |  ;Read all syntax files
  {
    SyntaxFileName = %A_LoopField%
    Gosub, ReadSyntaxFromFile       ;SyntaxFileName will become string with keywords
  }

  ;#############   Build language specific highlight for AHK   ##################
  StrgAHKwf = "AutoHotkey" Nocase
  StrgAHKwf = %StrgAHKwf% Line Comment = `;
  StrgAHKwf = %StrgAHKwf% Line Comment Preceding Chars = [~``]     ;to Escape Escaped ;
  StrgAHKwf = %StrgAHKwf% Escape Char = ``
  StrgAHKwf = %StrgAHKwf% String Chars = "                                                   ;"
  StrgAHKwf = %StrgAHKwf% Block Comment On = /*
  StrgAHKwf = %StrgAHKwf% Block Comment Off = */
  StrgAHKwf = %StrgAHKwf% File Extensions = ahk`n
  StrgAHKwf = %StrgAHKwf%/DeLimiters = *~`%+-!^&(){}=|\/:"'``;<>%A_Tab%,%A_Space%.`n         ;"
  StrgAHKwf = %StrgAHKwf%/Indent Strings = "{" ":" "("`n
  StrgAHKwf = %StrgAHKwf%/Unindent Strings = "}" "Return" "Else" ")"`n
  StrgAHKwf = %StrgAHKwf%/Open Fold Strings = "{"`n
  StrgAHKwf = %StrgAHKwf%/Close Fold Strings = "}"`n
  StrgAHKwf = %StrgAHKwf%/Function String = "`%[^t ]++^(:[^*^?BbCcKkOoPpRrZz0-9- ]++:*`::^)"`n   ; Hotstrings
  StrgAHKwf = %StrgAHKwf%/Function String 1 = "`%[^t ]++^([a-zA-Z0-9 #!^^&<>^*^~^$]+`::^)"`n     ; Hotkeys
  StrgAHKwf = %StrgAHKwf%/Function String 2 = "`%[^t ]++^([a-zA-Z0-9äöüß#_@^$^?^[^]]+:^)"`n      ; Subroutines
  StrgAHKwf = %StrgAHKwf%/Function String 3 = "`%[^t ]++^([a-zA-Z0-9äöüß#_@^$^?^[^]]+(*)^)"`n    ; Functions
  StrgAHKwf = %StrgAHKwf%/Function String 4 = "`%[^t ]++^(#[a-zA-Z]+ ^)"`n                       ; Directives

  Loop, Parse, SyntaxFileNameList, |      ;Add the keywords from syntax strings into their Sections
    {
      StrgAHKwf = %StrgAHKwf%/C%A_Index%"%A_LoopField%"   ;Section definition
      SyntaxString = %A_LoopField%             ;which Section/syntax
      Gosub, ParseSyntaxString                 ;Parse through string and add to list
    }

  ;#############   Add or Update Wordfile   #####################################

  ;Name of a file for temporary store the word file
  TemporaryUEwordFile = TempUEwordFile.txt
  FileDelete, %TemporaryUEwordFile%

  Loop, Read, %UEwordfile%, %TemporaryUEwordFile%   ;Read through Wordfile
    {
      StringLeft, WFdef, A_LoopReadLine, 2
      If WFdef = /L
        {
          StringSplit, WFname, A_LoopReadLine, "                  ;"
          LanguageName = %WFname2%
          LanguageNumber = %WFname1%
          StringTrimLeft,LanguageNumber,LanguageNumber,2
          If LanguageName = AutoHotkey         ;when AHK Section found, place new Section at same location
            {
              FileAppend, /L%LanguageNumber%%StrgAHKwf%
              AHKLanguageFound := True
            }
        }
      If LanguageName <> AutoHotkey            ;everything that does not belong to AHK, gets unchanged to file
          FileAppend, %A_LoopReadLine%`n
    }

  If not AHKLanguageFound                      ;when AHK Section not found, append AHK Section
    {
    	If (UEVersion < 15.00)
      	LanguageNumber += 1
      Else
      	LanguageNumber := UELanguage
      FileAppend, /L%LanguageNumber%%StrgAHKwf%, %TemporaryUEwordFile%
    }

  FileCopy, %UEwordfile%, %UEwordfile%.ahk.bak, 1    ;Create Backup of current wordfile
  FileMove, %TemporaryUEwordFile%, %UEwordfile%, 1       ;Replace wordfile with temporary file

  ; Tell user what has been done
  Question = `n`nWould you like to make UltraEdit the Default editor for AutoHotkey scripts (.ahk files)?
  If AHKLanguageFound
      MsgBox, 4,, The AutoHotkey-Syntax for UltraEdit has been updated in your wordfile:`n`n%UEwordfile%`n`nA backup has been created in the same folder.%Question%
  Else
      MsgBox, 4,, The AutoHotkey-Syntax for UltraEdit has been added to your wordfile:`n`n%UEwordfile%`n`nA backup has been created in the same folder.%Question%

  IfMsgBox, Yes
      RegWrite, REG_SZ, HKEY_CLASSES_ROOT, AutoHotkeyScript\Shell\Edit\Command,, %UEPath%\uedit32.exe "`%1"

  ExitApp  ; That's it, exit

  ;#############   SubRoutines   ################################################

  ReadSyntaxFromFile:
    TempString =
    Loop, Read , %AHKSyntax%\%SyntaxFileName%%SyntaxExtention%   ;read syntax file
      {
        StringLeft,Char, A_LoopReadLine ,1
        ;if line is comment, don't bother, otherwise add keyword to string
        If Char <> `;
          {
            ;only add first word in line
            Loop Parse, A_LoopReadLine, `,%A_Tab%%A_Space%(
              {
                TempString = %TempString%%A_LoopField%`n
                Break
              }
          }
      }
    %SyntaxFileName% = %TempString%                          ;Assign string to syntax filename
    Sort, %SyntaxFileName%, U                                ;Sort keywords in string
  Return

  ParseSyntaxString:
    Loop, Parse, %SyntaxString%, `n                 ;Parse through syntax string
      {
        StringLeft, Char, A_LoopField,1
        If (Char = PrevChar)                       ;add keyword to line when first character is same with previous keyword
            StrgAHKwf = %StrgAHKwf% %A_LoopField%
        Else                                       ;for every keyword with a new first letter, start a new row
            StrgAHKwf = %StrgAHKwf%`n%A_LoopField%
        PrevChar = %Char%                          ;remember first character of keyword
      }
  Return

  ;#############   END of File   ################################################
