# Microsoft Developer Studio Project File - Name="WinStyle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WinStyle - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinStyle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinStyle.mak" CFG="WinStyle - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinStyle - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WinStyle - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinStyle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Comctl32.lib Th32.lib /nologo /subsystem:windows /machine:I386 /OPT:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WinStyle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "_DEBUG"
# ADD RSC /l 0x40d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Comctl32.lib Th32.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "WinStyle - Win32 Release"
# Name "WinStyle - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\about.cpp
# End Source File
# Begin Source File

SOURCE=.\general.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\options.cpp
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# Begin Source File

SOURCE=.\tabs.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\about.h
# End Source File
# Begin Source File

SOURCE=.\general.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\options.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\tabs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bmp100.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp101.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp102.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1020.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1050.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1051.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1080.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1090.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1110.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp1130.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp480.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp481.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp485.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp486.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp488.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp489.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp490.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp491.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp493.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp497.bmp
# End Source File
# Begin Source File

SOURCE=.\delphi.ico
# End Source File
# Begin Source File

SOURCE=.\delphi2.ico
# End Source File
# Begin Source File

SOURCE=.\edit1.ico
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\ico1201.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\idd_hand.cur
# End Source File
# Begin Source File

SOURCE=.\main.ico
# End Source File
# Begin Source File

SOURCE=.\msdos.ico
# End Source File
# Begin Source File

SOURCE=.\mycomput.ico
# End Source File
# Begin Source File

SOURCE=.\progman.ico
# End Source File
# Begin Source File

SOURCE=.\ras.ico
# End Source File
# Begin Source File

SOURCE=.\regedit.ico
# End Source File
# Begin Source File

SOURCE=".\System-268.ico"
# End Source File
# Begin Source File

SOURCE=.\unknown.ico
# End Source File
# Begin Source File

SOURCE=.\vb5.ico
# End Source File
# Begin Source File

SOURCE=.\win2.ico
# End Source File
# Begin Source File

SOURCE=.\win_hide.ico
# End Source File
# Begin Source File

SOURCE=.\winstyle.ico
# End Source File
# End Group
# End Target
# End Project
