# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=WinStyle - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to WinStyle - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WinStyle - Win32 Release" && "$(CFG)" !=\
 "WinStyle - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinStyle.mak" CFG="WinStyle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinStyle - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WinStyle - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "WinStyle - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "WinStyle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\WinStyle.exe"

CLEAN : 
	-@erase ".\Release\WinStyle.exe"
	-@erase ".\Release\options.obj"
	-@erase ".\Release\main.obj"
	-@erase ".\Release\tabs.obj"
	-@erase ".\Release\general.obj"
	-@erase ".\Release\Script1.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/WinStyle.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x40d /d "NDEBUG"
# ADD RSC /l 0x40d /d "NDEBUG"
RSC_PROJ=/l 0x40d /fo"$(INTDIR)/Script1.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/WinStyle.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Comctl32.lib Th32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib Comctl32.lib Th32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/WinStyle.pdb" /machine:I386 /out:"$(OUTDIR)/WinStyle.exe" 
LINK32_OBJS= \
	"$(INTDIR)/options.obj" \
	"$(INTDIR)/main.obj" \
	"$(INTDIR)/tabs.obj" \
	"$(INTDIR)/general.obj" \
	"$(INTDIR)/Script1.res"

"$(OUTDIR)\WinStyle.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WinStyle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\WinStyle.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\WinStyle.exe"
	-@erase ".\Debug\tabs.obj"
	-@erase ".\Debug\options.obj"
	-@erase ".\Debug\general.obj"
	-@erase ".\Debug\main.obj"
	-@erase ".\Debug\Script1.res"
	-@erase ".\Debug\WinStyle.ilk"
	-@erase ".\Debug\WinStyle.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/WinStyle.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x40d /d "_DEBUG"
# ADD RSC /l 0x40d /d "_DEBUG"
RSC_PROJ=/l 0x40d /fo"$(INTDIR)/Script1.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/WinStyle.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Comctl32.lib Th32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib Comctl32.lib Th32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/WinStyle.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/WinStyle.exe" 
LINK32_OBJS= \
	"$(INTDIR)/tabs.obj" \
	"$(INTDIR)/options.obj" \
	"$(INTDIR)/general.obj" \
	"$(INTDIR)/main.obj" \
	"$(INTDIR)/Script1.res"

"$(OUTDIR)\WinStyle.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "WinStyle - Win32 Release"
# Name "WinStyle - Win32 Debug"

!IF  "$(CFG)" == "WinStyle - Win32 Release"

!ELSEIF  "$(CFG)" == "WinStyle - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Script1.rc
DEP_RSC_SCRIP=\
	".\unknown.ico"\
	".\ico1201.ico"\
	".\vb5.ico"\
	".\progman.ico"\
	".\regedit.ico"\
	".\bmp480.bmp"\
	".\bmp481.bmp"\
	".\bmp486.bmp"\
	".\bmp489.bmp"\
	".\bmp490.bmp"\
	".\bmp491.bmp"\
	".\bmp493.bmp"\
	".\bmp497.bmp"\
	".\bmp1020.bmp"\
	".\bmp102.bmp"\
	".\bmp101.bmp"\
	".\bmp100.bmp"\
	".\bmp1050.bmp"\
	".\bmp1051.bmp"\
	".\bmp1080.bmp"\
	".\bmp1090.bmp"\
	".\bmp1110.bmp"\
	".\bmp1130.bmp"\
	".\bmp485.bmp"\
	".\bmp488.bmp"\
	".\delphi.ico"\
	".\delphi2.ico"\
	".\icon1.ico"\
	".\ico00001.ico"\
	".\ico00002.ico"\
	".\ico00003.ico"\
	".\ico00004.ico"\
	".\ico00005.ico"\
	".\ico00006.ico"\
	".\ico00007.ico"\
	".\ico00008.ico"\
	".\win2.ico"\
	".\win_hide.ico"\
	".\winstyle.ico"\
	".\edit1.ico"\
	".\ico00009.ico"\
	".\ico00010.ico"\
	".\ico00011.ico"\
	".\ico00012.ico"\
	".\ico00013.ico"\
	".\ico00014.ico"\
	".\ico00015.ico"\
	".\msdos.ico"\
	".\System-268.ico"\
	

"$(INTDIR)\Script1.res" : $(SOURCE) $(DEP_RSC_SCRIP) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\main.cpp
DEP_CPP_MAIN_=\
	".\general.h"\
	".\tabs.h"\
	".\main.h"\
	".\options.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tabs.cpp
DEP_CPP_TABS_=\
	".\general.h"\
	".\tabs.h"\
	

"$(INTDIR)\tabs.obj" : $(SOURCE) $(DEP_CPP_TABS_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\general.cpp
DEP_CPP_GENER=\
	".\general.h"\
	

"$(INTDIR)\general.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\options.cpp
DEP_CPP_OPTIO=\
	".\general.h"\
	".\options.h"\
	

"$(INTDIR)\options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
