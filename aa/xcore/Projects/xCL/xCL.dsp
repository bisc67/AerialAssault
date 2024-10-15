# Microsoft Developer Studio Project File - Name="xCL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=xCL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xCL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xCL.mak" CFG="xCL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xCL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "xCL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/xCL", WOSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xCL - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Installing...
PostBuild_Cmds=copy Release\xCL.exe "C:\Program Files\Microsoft Visual Studio\VC98\Bin\CL.exe" > nul	copy Release\xCL.exe "C:\Program Files\Microsoft Visual Studio\VC98\Bin\LINK.exe" > nul
# End Special Build Tool

!ELSEIF  "$(CFG)" == "xCL - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Installing...
PostBuild_Cmds=copy Debug\xCL.exe "C:\Program Files\Microsoft Visual Studio\VC98\Bin\CL.exe" > nul	copy Debug\xCL.exe "C:\Program Files\Microsoft Visual Studio\VC98\Bin\LINK.exe" > nul
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "xCL - Win32 Release"
# Name "xCL - Win32 Debug"
# Begin Group "Template DSPs"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\Tools\Project Templates\Templates\App(Entropy).dsp"
# End Source File
# Begin Source File

SOURCE="..\..\Tools\Project Templates\Templates\App(TextMode).dsp"
# End Source File
# Begin Source File

SOURCE="..\..\Tools\Project Templates\Templates\Lib.dsp"
# End Source File
# Begin Source File

SOURCE="..\..\Tools\Project Templates\Templates\PS2_IOP_IRX.dsp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\CmdLineData.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdLineData.hpp
# End Source File
# Begin Source File

SOURCE=.\CmdLnTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdLnTokenizer.hpp
# End Source File
# Begin Source File

SOURCE=.\Inevitable.cpp
# End Source File
# Begin Source File

SOURCE=.\Inevitable.hpp
# End Source File
# Begin Source File

SOURCE=.\TokenList.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenList.hpp
# End Source File
# Begin Source File

SOURCE=.\xCL.cpp
# End Source File
# Begin Source File

SOURCE=.\xCL.hpp
# End Source File
# Begin Source File

SOURCE=.\xCL_CodeWarrior.cpp
# End Source File
# Begin Source File

SOURCE=.\xCL_CodeWarrior.hpp
# End Source File
# Begin Source File

SOURCE=.\xCL_GCN_SN.cpp
# End Source File
# Begin Source File

SOURCE=.\xCL_GCN_SN.hpp
# End Source File
# Begin Source File

SOURCE=.\xCL_PS2_SN.cpp
# End Source File
# Begin Source File

SOURCE=.\xCL_PS2_SN.hpp
# End Source File
# Begin Source File

SOURCE=.\xCL_Thru.cpp
# End Source File
# Begin Source File

SOURCE=.\xCL_Thru.hpp
# End Source File
# End Target
# End Project
